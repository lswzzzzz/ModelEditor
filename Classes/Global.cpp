#include "Global.h"
#include "FontChina.h"
#include <cstdarg>
#include <qtextcursor.h>
#include <atlstr.h>
#include <QTextStream>
#include <QJsonDocument>

QWidget* glWidget = NULL;
Layer* g_scene = NULL;
extern Layer* g_layer = NULL;
QObject* g_editor = NULL;
AppDelegate* g_app = NULL;
HWND g_cocos_HWND = NULL;
QPlainTextEdit* logText = NULL;

void addConsoleInfo(std::string str)
{
	QTextCursor cursor = logText->textCursor();
	cursor.movePosition(QTextCursor::End);
	logText->setTextCursor(cursor);
	logText->insertPlainText(FontChina::G2U((str + "\n").c_str()));
	cursor.movePosition(QTextCursor::End);
	logText->setTextCursor(cursor);
	logText->ensureCursorVisible();
}

void addConsoleInfo(char* fmt, ...)
{
	char buf[4096];
	char* p = buf;
	va_list args;
	va_start(args, fmt);
	vsprintf(p, fmt, args);
	va_end(args);
	std::string str;
	str = buf;
	str += "\n";
	QTextCursor cursor = logText->textCursor();
	cursor.movePosition(QTextCursor::End);
	logText->setTextCursor(cursor);
	logText->insertPlainText(FontChina::G2U(str.c_str()));
	cursor.movePosition(QTextCursor::End);
	logText->setTextCursor(cursor);
	logText->ensureCursorVisible();
}

void clearConsoleInfo()
{
	logText->clear();
}

