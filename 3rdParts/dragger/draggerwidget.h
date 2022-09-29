#ifndef DRAGGERWIDGET_H
#define DRAGGERWIDGET_H

#include <QWidget>
#include <QMainWindow>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <ostream>

//QT_BEGIN_NAMESPACE
//namespace Ui { class DraggerWidget; }
//QT_END_NAMESPACE

class DraggerWidget : public QMainWindow
{
    Q_OBJECT
public:
    enum Type {
        TYPE_NOTHING = 0x0,
        TYPE_FILE = 0x1,
        TYPE_FOLDER = 0x2,
        TYPE_MIX = TYPE_FILE | TYPE_FOLDER // 混合拖入
    };
    /**
     * @brief The DragObjects_t struct 拖入的文件和文件夹信息
     */
    struct DragObjects_t {
        enum Type type{TYPE_NOTHING};
        QStringList file_list{};
        QStringList folder_list{};
        inline size_t size() const {return file_list.size() + folder_list.size();}
        inline bool isEmpty() const {return size() <= 0;}
    private:
        friend DraggerWidget;
        friend QDebug operator<<(QDebug os, const DraggerWidget::DragObjects_t& d);
        inline void clear() {file_list.clear(); folder_list.clear(); type = TYPE_NOTHING;}

        inline const char* str() const {
            QStringList list;
            list << "TYPE:        " + (type & TYPE_FILE ? QString("File ") : QString("")) + (type & TYPE_FOLDER ? QString("Folder") : QString(""));
            list << "TOTAL_SIZE:  " + QString::number(size());
            list << "FILE_LIST:   " + file_list.join(";");
            list << "FOLDER_LIST: " + folder_list.join(";");
            return list.join('\n').toLocal8Bit().data();
        }
    };

    /*****************************新接口*****************************/
signals:
    void sthDragged(const DragObjects_t& d);

public:
    DraggerWidget(QWidget *parent = nullptr);

    ~DraggerWidget();
    /*****************************接口*****************************/

private:
    DragObjects_t _last_d;

private:
    /**
     * @brief _initWidget 初始化窗口
     */
    void _initWidget();

protected:
    /**
     * @brief dragEnterEvent 重写拖拽进入事件处理
     * @param event
     */
    virtual void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief dragLeaveEvent 重写拖拽离开事件处理
     * @param event
     */
    virtual void dragLeaveEvent(QDragLeaveEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief dropEvent重写拖拽放下事件处理
     * @param event
     */
    virtual void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
};

QDebug operator<<(QDebug os, const DraggerWidget::DragObjects_t& d);

#endif // DRAGGERWIDGET_H
