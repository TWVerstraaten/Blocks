#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>
#include <QWidget>
#include <QStringList>

namespace view2 {
    class TextEdit : public QTextEdit {
        Q_OBJECT
      public:
        explicit TextEdit(QWidget* parent = nullptr);

        void keyPressEvent(QKeyEvent* e) override;

        [[nodiscard]] QStringList contents() const;

      private:
        void setHeight();

      signals:

        void tabPressed();
        void backTabPressed();
    };
} // namespace view2

#endif // TEXTEDIT_H
