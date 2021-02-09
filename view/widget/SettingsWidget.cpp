//
// Created by teunv on 2/2/2021.
//

#include "SettingsWidget.h"

#include "../../Io/SettingsManager.h"
#include "../../audio/AudioSettings.h"
#include "../../misc/defines.h"
#include "../FontManager.h"

#include <QComboBox>
#include <QFormLayout>
#include <QLabel>

namespace view::widget {

    template <typename T>
    static void set(const T& value, const QString& nameInIni) {
        io::SettingsManager::settings().setValue(nameInIni, value);
    }

    template <typename T>
    static void addSettingWithSlider(QFormLayout* m_layout, QWidget* parent, const QString& name, const T value, T min, T max, void (*setter)(T)) {
        auto* label = new QLabel(name, parent);
        label->setFont(FontManager::font(FONT_ENUM::ANON_PRO_ITALIC, 13));
        auto* slider = new QSlider(Qt::Horizontal, parent);
        assert(slider->minimum() == 0);
        slider->setMaximum(100);

        slider->setValue(100.0 * (min + value) / static_cast<double>(max - min));
        QWidget::connect(slider, &QSlider::valueChanged, [min, max, name, setter](int value) {
            T newValue = static_cast<T>(min + (value / 100.0) * (max - min));
            (*setter)(newValue);
            io::SettingsManager::writeValueToFile(newValue, name);
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

    static void addDivider(QFormLayout* layout, QWidget* parent, const QString& string) {
        auto* l = new QLabel(string, parent);
        l->setFont(FontManager::font(FONT_ENUM::UBUNTU_MONO_ITALIC, 17));
        layout->addRow(l, new QWidget(parent));
    }

    SettingsWidget::SettingsWidget(QWidget* parent, Window* window) : QWidget(parent) {
        connect(this, &SettingsWidget::windowModeChanged, window, &Window::setWindowMode);
        setMaximumWidth(500);
        auto* layout = new QFormLayout(this);

        addDivider(layout, this, "Audio Settings");
        addSettingWithSlider(
            layout, this, "Sound fx volume", audio::AudioSettings::soundEffectsVolume(), 0, 100, &audio::AudioSettings::setSoundEffectVolume);
        addSettingWithSlider(layout, this, "Music volume", audio::AudioSettings::musicVolume(), 0, 100, &audio::AudioSettings::setMusicVolume);

        addDivider(layout, this, "window Settings");
        addSettingWithComboBox(layout,
                               this,
                               "Window mode",
                               {{"Maximized", D_THIS_FUNC(emit windowModeChanged(WINDOW_MODE::MAXIMIZED);)},
                                {"Fullscreen", D_THIS_FUNC(emit windowModeChanged(WINDOW_MODE::FULL_SCREEN);)},
                                {"Borderless", D_THIS_FUNC(emit windowModeChanged(WINDOW_MODE::BORDERLESS);)}});
    }

} // namespace view::widget
