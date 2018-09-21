#include "imageviewer.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QMenu>
#include <QtEvents>

ImageViewer::ImageViewer(bool isContextualMenu, QWidget *parent)
	: QGraphicsView(parent)
	, zoomFactor(1.15)
	, zoomCtrlFactor(1.01)
{
	// Allow mouse tracking even if no button is pressed
	setMouseTracking(true);

	setScene(&scene);

	// Update all the view port when needed, otherwise, the drawInViewPort may experience trouble
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

	// When zooming, the view stay centered over the mouse
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

	setResizeAnchor(QGraphicsView::AnchorViewCenter);

	if (isContextualMenu) {
		setContextMenuPolicy(Qt::CustomContextMenu);
		connect(this, &ImageViewer::customContextMenuRequested, this, &ImageViewer::showContextMenu);
	}

	pixmapItem = scene.addPixmap(mImage);
}

ImageViewer::~ImageViewer()
{
}

void ImageViewer::onImageRotate()
{
	QMatrix rm;
	rm.rotate(90);
	mImage = mImage.transformed(rm);
	pixmapItem->setPixmap(mImage);

	scene.setSceneRect(QRect(QPoint(0, 0), mImage.size()));
	fitInView(scene.sceneRect(), Qt::KeepAspectRatio);

	imageSize = mImage.size();
}

// Display contextual menu
void ImageViewer::showContextMenu(const QPoint &pos)
{
	// Get the mouse position in the scene
	QPoint globalPos = mapToGlobal(pos);
	QMenu contextMenu;
	contextMenu.addAction("Reset view", this, SLOT(fitImage()));
	contextMenu.exec(globalPos);
}

// Set or update the image in the scene
void ImageViewer::setImage(const QPixmap &image)
{
	rotateState = 0;
	mImage = image;
	pixmapItem->scale();
	pixmapItem->setPixmap(mImage);

	// Resize the scene (needed if the new image is smaller)
	scene.setSceneRect(QRect(QPoint(0, 0), image.size()));
	fitInView(scene.sceneRect(), Qt::KeepAspectRatio);
	imageSize = image.size();
}

// Fit the image in the widget
void ImageViewer::fitImage()
{
	Qt::ScrollBarPolicy currentHorizontalPolicy = horizontalScrollBarPolicy();
	Qt::ScrollBarPolicy currentverticalPolicy = verticalScrollBarPolicy();

	// Disable scroll bar to avoid a margin around the image
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	// Fit the scene in the QGraphicsView
	fitInView(scene.sceneRect(), Qt::KeepAspectRatio);

	// Restaure scroll bar policy
	setHorizontalScrollBarPolicy(currentHorizontalPolicy);
	setVerticalScrollBarPolicy(currentverticalPolicy);
}

// Called when a mouse button is pressed
void ImageViewer::mousePressEvent(QMouseEvent *event)
{
	// Drag mode : change the cursor's shape
	if (event->button() == Qt::LeftButton) {
		setDragMode(QGraphicsView::ScrollHandDrag);
	}
	//    if (event->button() == Qt::RightButton) fitImage();
	QGraphicsView::mousePressEvent(event);
}

// Called when a mouse button is pressed
void ImageViewer::mouseReleaseEvent(QMouseEvent *event)
{
	// Exit drag mode : change the cursor's shape
	if (event->button() == Qt::LeftButton) {
		setDragMode(QGraphicsView::NoDrag);
	}
	QGraphicsView::mouseReleaseEvent(event);
}

#ifndef QT_NO_WHEELEVENT

// Call when there is a scroll event
void ImageViewer::wheelEvent(QWheelEvent *event)
{
	// When zooming, the view stay centered over the mouse
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

	double factor = (event->modifiers() & Qt::ControlModifier) ? zoomCtrlFactor : zoomFactor;
	if (event->delta() > 0 && (imageSize.height() * transform().m11()) < 10000) {
		// Zoom in
		scale(factor, factor);
	} else if ((imageSize.height() * transform().m11()) > 50) {
		// Zooming out
		scale(1.0 / factor, 1.0 / factor);
	}
	event->accept();
}

#endif

// Overload the mouse MoveEvent to display the tool tip
void ImageViewer::mouseMoveEvent(QMouseEvent *event)
{
	// Get the coordinates of the mouse in the scene
	QPointF imagePoint = mapToScene(QPoint(event->x(), event->y()));
	// Call the function that create the tool tip
	setToolTip(setToolTipText(QPoint((int)imagePoint.x(), (int)imagePoint.y())));
	// Call the parent's function (for dragging)
	QGraphicsView::mouseMoveEvent(event);
}

// Overload the function to draw over the image
void ImageViewer::drawForeground(QPainter *painter, const QRectF &)
{

	// Call the function to draw over the image
	drawOnImage(painter, imageSize);

	// Reset transformation and call the function draw in the view port
	painter->resetTransform();

	// Call the function to draw in the view port
	drawInViewPort(painter, viewport()->size());
}

// Overloaded functionthat catch the resize event
void ImageViewer::resizeEvent(QResizeEvent *event)
{

	// First call, the scene is created
	if (event->oldSize().width() == -1 || event->oldSize().height() == -1) {
		return;
	}

	// Get the previous rectangle of the scene in the viewport
	QPointF P1 = mapToScene(QPoint(0, 0));
	QPointF P2 = mapToScene(QPoint(event->oldSize().width(), event->oldSize().height()));

	// Stretch the rectangle around the scene
	if (P1.x() < 0)
		P1.setX(0);
	if (P1.y() < 0)
		P1.setY(0);
	if (P2.x() > scene.width())
		P2.setX(scene.width());
	if (P2.y() > scene.height())
		P2.setY(scene.height());

	// Fit the previous area in the scene
	fitInView(QRect(P1.toPoint(), P2.toPoint()), Qt::KeepAspectRatio);
}

// Define the virtual function to avoid the "unused parameter" warning
QString ImageViewer::setToolTipText(QPoint imageCoordinates)
{
	(void)imageCoordinates;
	return QString("");
}

// Define the virtual function to avoid the "unused parameter" warning
void ImageViewer::drawOnImage(QPainter *, QSize)
{
}

// Define the virtual function to avoid the "unused parameter" warning
void ImageViewer::drawInViewPort(QPainter *, QSize)
{
}
