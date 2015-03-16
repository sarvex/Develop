#ifndef IMAGE_BUTTON
#define IMAGE_BUTTON

#include <QtWidgets/QPushButton>

class ImageButton : public QPushButton {
	Q_OBJECT

public:
	ImageButton (const QString& text,
				 const QString& normalImage,
				 bool hyperlink,
				 QWidget *parent = 0);

	ImageButton (const QString& text,
				 const QString& normalImage,
				 const QString& hoverImage,
				 bool hyperlink,
				 QWidget *parent = 0);

	ImageButton (const QString& text,
				 const QString& normalImage,
				 const QString& hoverImage,
				 const QString& pressImage,
				 bool hyperlink,
				 QWidget* parent = 0);

	ImageButton (const QString& text,
				 const QString& normalImage,
				 const QString& hoverImage,
				 const QString& pressImage,
				 const QString& disableImage,
				 bool hyperlink,
				 QWidget* parent = 0);

	bool setSelect (bool status);
	bool setSelect ();

signals:

public slots:

protected:
	virtual void paintEvent(QPaintEvent * event);
	virtual void enterEvent(QEvent * event);
	virtual void leaveEvent(QEvent * event);

private:
	QString text;

	QImage normalImage;
	QImage hoverImage;
	QImage pressImage;
	QImage disableImage;

	bool hyperlink;
	bool enable;
	bool hover;
	bool press;
};

#endif // IMAGE_BUTTON
