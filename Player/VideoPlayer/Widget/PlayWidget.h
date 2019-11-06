#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <string>
#include "OutDevice.h"

class PlayWidget : public QWidget,virtual OutDevice
{
    Q_OBJECT
public:
	//param1:���ؼ�ָ��
	//param2:�������ļ�·��
    explicit PlayWidget(QWidget *parent, const std::string & strStreamFile = "");

    ~PlayWidget();

	virtual void Draw(const QImage& img);
protected:
    void paintEvent(QPaintEvent *event);
private:
	QVBoxLayout *mVLayout;
	QWidget *mEmptyWidget;
	QWidget *mpControlWidget;
	QImage mImage;
};

#endif // MYWIDGET_H
