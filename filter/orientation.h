#ifndef ORIENTATION_H
#define ORIENTATION_H

#include "interface.h"

// Orientation defines folowing directions of
// text reading
//
// 1: rigth/down
//  	->
//		|
//
// 2: left/down
//		<-
//		|
//
// 3: rigth/up
//		|
//		->
//
// 4: left/up
//		|
//		<-
//

class Orientation : public FilterInterface {
public:
	enum Horizontal {
		LeftToRight, // ->
		RightToLeft // <-
	};

	enum Vertical {
		UpToDown,
		DownToUp
	};

	Orientation();
	QList<Page> processPages(QList<Page> &pages) override;
	QString process(QString &text) override;
	void settingsUi(QWidget *parent) override;

private:
	static QString arg1;
	static QString arg2;
};

#endif // ORIENTATION_H
