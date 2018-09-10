#include "reverse.h"

#include <QString>
#include <algorithm>

QString Reverse::process(QString &text)
{
	std::reverse(text.begin(), text.end());
	return text;
}
