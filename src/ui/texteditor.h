#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QFont>
#include <QLineEdit>
#include <QObject>
#include <QTextEdit>
#include <QWidget>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class LineNumberArea;

class TextEditor : public QTextEdit {
	Q_OBJECT

public:
	explicit TextEditor(QWidget *parent);
	~TextEditor() {}

	void lineNumberAreaPaintEvent(QPaintEvent *event);
	int lineNumberAreaWidth();

protected:
	void resizeEvent(QResizeEvent *event);
	int getFirstVisibleBlockId();

signals:
	void plainTextChanged(const QString &text);
	void textFound(bool found);

public slots:
	void updateLineNumberAreaWidth(int);
	void on_actionWrapLine_toggled(bool wrapLine);
	void searchedTextChanged(const QString searchedText);
	void findNextText(const QString &text);
	void findPreviousText(const QString &text);
	void replaceNextText(const QString &search, const QString &replace);
	void replaceAllText(const QString &search, const QString &replace);

private slots:
	void on_pageTextChanged();
	void updateLineNumberArea(QRectF /*rect_f*/);
	void updateLineNumberArea(int /*slider_pos*/);
	void updateLineNumberArea();
	//void highlightCurrentLine();

private:
	bool isFirstTime;
	QWidget *lineNumberArea;
	QList<QTextCursor> mSelection;
};

class LineNumberArea : public QWidget {
public:
	LineNumberArea(TextEditor *editor)
		: QWidget(editor)
	{
		mTextEditor = editor;
	}

	QSize sizeHint() const
	{
		return QSize(mTextEditor->lineNumberAreaWidth(), 0);
	}

protected:
	void paintEvent(QPaintEvent *event)
	{
		mTextEditor->lineNumberAreaPaintEvent(event);
	}

private:
	TextEditor *mTextEditor;
};

#endif // TEXTEDITOR_H
