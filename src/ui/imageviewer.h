#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "qgraphicsview.h"
#include <QPixmap>

class QGraphicsPixmapItem;
class QMouseEvent;

class ImageViewer : public QGraphicsView {
	Q_OBJECT
public:
	//explicit
	ImageViewer(bool isContextualMenu = true, QWidget *parent = 0);
	~ImageViewer();

	void setImage(const QPixmap &image);

public slots:
	void onImageRotate();
	void fitImage();

protected:
	/*!
     * \brief drawOnImage           Overload this function to draw over the image
     * \param painter               Painter for drawing
     * \param imageSize             Size of the image
     */
	virtual void drawOnImage(QPainter *painter, QSize imageSize);

	/*!
     * \brief drawOnImage           Overload this function to draw in the view port
     * \param painter               Painter for drawing
     * \param imageSize             Size of the view port
     */
	virtual void drawInViewPort(QPainter *painter, QSize portSize);

	/*!
     * \brief drawForeground        Drawing he foreground of the scene
     *                              When overloading this function, you must call it parent before
     *                              exiting the function, otherwise drawOnImage and drawInViewPort
     *                              will not work properly
     * \param painter               Painter for drawing
     * \param rect                  The area of the scene displayed in the view port
     */
	virtual void drawForeground(QPainter *painter, const QRectF &rect);

	/*!
     * \brief setToolTipText        Display the tool tip over the mouse
     * \param imageCoordinates      Coordinates of the mouse in the image's frame
     * \return                      The function returns the QString that is displayed over the image
     */
	virtual QString setToolTipText(QPoint imageCoordinates);

	/*!
     * \brief mousePressEvent       Overload this function to process mouse button pressed
     * \param event                 mouse event
     */
	virtual void mousePressEvent(QMouseEvent *event);

	/*!
     * \brief mouseReleaseEvent     Overload this function to process mouse button released
     * \param event                 mouse event
     */
	virtual void mouseReleaseEvent(QMouseEvent *event);

	/*!
     * \brief wheelEvent            Overload this function to process mouse wheel event
     * \param event                 mouse wheel event
     */
	virtual void wheelEvent(QWheelEvent *event);

	/*!
     * \brief mouseMoveEvent        Overload this function to process mouse moves
     * \param event                 mouse move event
     */
	virtual void mouseMoveEvent(QMouseEvent *event);

	/*!
     * \brief resizeEvent        Overload this function to process the window resizing event
     * \param event              resize event
     */
	virtual void resizeEvent(QResizeEvent *event);

protected slots:

	/*!
     * \brief showContextMenu       Display the contextual menu (on right click)
     * \param pos                   Position of the mouse in the widget
     */
	virtual void showContextMenu(const QPoint &pos);

private:
	// Scene where the image is drawn
	QGraphicsScene scene;

	// Pixmap item containing the image
	QGraphicsPixmapItem *pixmapItem;

	// Zoom factor
	double zoomFactor;

	// Zoom factor when the CTRL key is pressed
	double zoomCtrlFactor;

	// Size of the current image
	QSize imageSize;
	QPixmap mImage;
	int rotateState;
};

#endif // IMAGEVIEWER_H
