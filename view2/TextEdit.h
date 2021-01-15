#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>
#include <QWidget>
#include <string>
#include <vector>

namespace view2 {
    class TextEdit : public QTextEdit {
        Q_OBJECT
      public:
        explicit TextEdit(QWidget* parent = nullptr);

        void keyPressEvent(QKeyEvent* event) override;

        [[nodiscard]] std::vector<std::string> contents() const;
        void                                   setHeight();

      protected:
        [[nodiscard]] QSize sizeHint() const override;

      private:
      signals:

        void tabPressed();
        void backTabPressed();
    };
} // namespace view2

#endif // TEXTEDIT_H
