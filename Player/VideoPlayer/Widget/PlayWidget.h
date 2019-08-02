#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <string>

class PlayWidget : public QWidget
{
    Q_OBJECT
public:
	//param1:���ؼ�ָ��
	//param2:�������ļ�·��
    explicit PlayWidget(QWidget *parent, const std::string & strStreamFile = "");

    ~PlayWidget();

	void Draw(const QImage& img);
protected:
    void paintEvent(QPaintEvent *event);
private:
	QVBoxLayout *mVLayout;
	QWidget *mEmptyWidget;
	QWidget *mProgressWidget;
	QWidget *mpControlWidget;
	QImage mImage;
};

#endif // MYWIDGET_H
