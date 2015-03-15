#include "ImageButton.h"

#include <QPainter>

ImageButton::ImageButton (
		const QString& text,
		const QString& normalImage,
		const QString& hoverImage,
		const QString& pressImage,
		const QString& disableImage,
		bool hyperlink,
		QWidget* parent) :
	QPushButton (parent),
	text (text),
	normalImage (normalImage),
	hoverImage (hoverImage),
	pressImage (pressImage),
	disableImage (disableImage),
	hyperlink (hyperlink),
	enable (true),
	hover (false),
	press (false)  {
}

ImageButton::ImageButton (
		const QString& text,
		const QString& normalImage,
		const QString& hoverImage,
		const QString& pressImage,
		bool hyperlink,
		QWidget* parent) :
	QPushButton (parent),
	text (text),
	normalImage (normalImage),
	hoverImage (hoverImage),
	pressImage (pressImage),
	disableImage (normalImage),
	hyperlink (hyperlink),
	enable (true),
	hover (false),
	press (false) {
}

ImageButton::ImageButton (
		const QString& text,
		const QString& normalImage,
		const QString& hoverImage,
		bool hyperlink,
		QWidget* parent) :
	QPushButton(parent),
	text (text),
	normalImage (normalImage),
	hoverImage (hoverImage),
	pressImage (normalImage),
	disableImage (normalImage),
	hyperlink (hyperlink),
	enable (true),
	hover (false),
	press (false) {
}

ImageButton::ImageButton (const QString& text,
						  const QString& normalImage,
						  bool hyperlink,
						  QWidget* parent) :
	QPushButton(parent),
	text (text),
	normalImage (normalImage),
	hoverImage (normalImage),
	pressImage (normalImage),
	disableImage (normalImage),
	hyperlink (hyperlink),
	enable (true),
	hover (false),
	press (false) {
}


void
ImageButton::paintEvent (
		QPaintEvent* event) {
	Q_UNUSED(event);

	QPainter painter(this);

	if (true == this->press  && this->isEnabled()) {

		painter.drawImage(QPoint(0,0), this->pressImage);

	} else if (true == this->hover && this->isEnabled()) {

		painter.drawImage(QPoint(0,0), this->hoverImage);

	} else if (true == this->isEnabled ()){

		painter.drawImage(QPoint(0,0), this->normalImage);

	} else {

		painter.drawImage (QPoint(0,0), this->disableImage);
	}

	if (false == this->text.isEmpty()) {

		painter.setRenderHint (QPainter::Antialiasing);
		painter.setPen (Qt::white);
		painter.drawText (QRect(0,0, this->width (), this->height ()),
						  Qt::AlignCenter,
						  this->text);
	}

	painter.end();
}

void
ImageButton::enterEvent (
		QEvent * event) {

	if (this->hyperlink) {
		this->setCursor (Qt::PointingHandCursor);
	}

	this->hover = true;
	this->update();

	QPushButton::enterEvent(event);

}

void
ImageButton::leaveEvent(
		QEvent * event) {

	if (this->hyperlink) {
		this->setCursor (Qt::ArrowCursor);
	}

	this->hover = false;
	this->update();

	QPushButton::leaveEvent(event);
}

bool
ImageButton::setSelect (
		bool status) {

	this->press = status;
	this->update();

	return(true);
}

bool
ImageButton::setSelect () {

	this->press = !this->press;
	this->update();

	return(true);
}
