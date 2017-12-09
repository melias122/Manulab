#include "filter/reverse.h"

#include "qstring.h"
#include "algorithm"

QString Reverse::process(QString &text)
{
	std::reverse(text.begin(), text.end());
	return text;
}
