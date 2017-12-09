#ifndef REVERSE_H
#define REVERSE_H

#include "filter/interface.h"

class Reverse : public FilterInterface
{
public:
	QString process(QString &text) override;
};

#endif // REVERSE_H
