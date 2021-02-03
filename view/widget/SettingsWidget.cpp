//
// Created by teunv on 2/2/2021.
//

#include "SettingsWidget.h"

#include "../../Io/SettingsManager.h"
#include "../../audio/AudioSettings.h"
#include "../../misc/defines.h"
#include "../FontManager.h"

#include <QComboBox>
#include <QDebug>
#include <QLabel>

namespace view::widget {

    template <typename T>
    static void set(const T& value, const QString& nameInIni) {
        io::SettingsManager::settings().setValue(nameInIni, value);
    }

    double getValue(const QString& name) {
        return io::SettingsManager::settings().value(name).toFloat();
    }

    template <typename T>
    void readOrWrite(const QString& name, const T fallBack, void (*f)(T)) {
        if (io::SettingsManager::settings().contains(name)) {
            f(getValue(name));
        } else {
            set(fallBack, name);
            assert(io::SettingsManager::settings().contains(name));
        }
    }

    template <typename T>
    static void addSettingWithSlider(QFormLayout* m_layout, QWidget* parent, const QString& name, const T fallBack, T min, T max, void (*f)(T)) {
        auto* label = new QLabel(name, parent);
        label->setFont(FontManager::font(FONT_ENUM::ANON_PRO_ITALIC, 13));
        auto* slider = new QSlider(Qt::Horizontal, parent);
        assert(slider->minimum() == 0);
        slider->setMaximum(100);

        readOrWrite(name, fallBack, f);

        slider->setValue(100.0 * (min + fallBack) / (max - min));
        QWidget::connect(slider, &QSlider::valueChanged, [min, max, name, f, slider](int value) {
            T newValue = static_cast<T>(min + (value / 100.0) * (max - min));
            (*f)(newValue);
            set(newValue, name);
        });
        m_layout->addRow(label, slider);
    }

    void addSettingWithComboBox(QFormLayout*                                            layout,
                                QWidget*                                                parent,
                                const QString&                                          name,
                                const QList<QPair<QString, std::function<void(void)>>>& entries) {
        assert(not entries.empty());
        auto* label = new QLabel(name, parent);
        label->setFont(FontManager::font(FONT_ENUM::ANON_PRO_ITALIC, 13));

        if (not io::SettingsManager::settings().contains(name)) {
            io::SettingsManager::settings().setValue(name, 0);
        }

        assert(io::SettingsManager::settings().contains(name));
        const quint32 value = io::SettingsManager::settings().value(name).toUInt();
        assert(value < static_cast<quint32>(entries.size()));
        (entries.at(value).second)();

        auto* comboBox = new QComboBox(parent);
        for (const auto& [entry, func] : entries) {
            comboBox->addItem(entry);
        }
        comboBox->setCurrentIndex(value);

        QWidget::connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
            assert(index < entries.size());
            (entries.at(index).second)();
            set(index, name);
        });
        layout->addRow(label, comboBox);
    }

    SettingsWidget::SettingsWidget(QWidget* parent, Window* window) : QWidget(parent) {
        connect(this, &SettingsWidget::windowModeChanged, window, &Window::setWindowMode);
        setMaximumWidth(500);
        setFont(FontManager::font(FONT_ENUM::ANON_PRO_BOLD, 13));
        m_formLayout = new QFormLayout(this);

        addDivider("Audio Settings");
        addSettingWithSlider(
            m_formLayout, this, "Sound fx volume", audio::AudioSettings::soundEffectsVolume(), 0.0, 1.0, &audio::AudioSettings::setSoundEffectVolume);
        addSettingWithSlider(m_formLayout, this, "Music volume", audio::AudioSettings::musicVolume(), 0, 100, &audio::AudioSettings::setMusicVolume);

        addDivider("window Settings");
        addSettingWithComboBox(m_formLayout,
                               this,
                               "Window mode",
                               {

                                   {"Maximized", D_THIS_FUNC(emit windowModeChanged(WINDOW_MODE::MAXIMIZED);)},
                                   {"Fullscreen", D_THIS_FUNC(emit windowModeChanged(WINDOW_MODE::FULL_SCREEN);)},
                                   {"Borderless", D_THIS_FUNC(emit windowModeChanged(WINDOW_MODE::BORDERLESS);)}

                               });

        setLayout(m_formLayout);
    }

    void SettingsWidget::addDivider(const QString& string) {
        auto* l = new QLabel(string, this);
        l->setFont(FontManager::font(FONT_ENUM::ANON_PRO_BOLD, 15));
        m_formLayout->addRow(l, new QWidget(this));
    }

} // namespace view::widget