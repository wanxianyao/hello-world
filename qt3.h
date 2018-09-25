#ifndef QT3_H
#define QT3_H

#include <QtWidgets/QMainWindow>
#include "ui_qt3.h"

class qt3 : public QMainWindow
{
	Q_OBJECT

public:
	qt3(QWidget *parent = 0);
	~qt3();

private:
	Ui::qt3Class ui;
};

#endif // QT3_H
