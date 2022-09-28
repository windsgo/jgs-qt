#ifndef DRAGGERWIDGET_H
#define DRAGGERWIDGET_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <ostream>

QT_BEGIN_NAMESPACE
namespace Ui { class DraggerWidget; }
QT_END_NAMESPACE

class DraggerWidget : public QWidget
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
//        inline QDebug str(QDebug os) const {
//            os << "TYPE:        " << (type & TYPE_FILE ? "File " : "") << (type & TYPE_FOLDER ? "Folder" : "") << '\n';
//            os << "TOTAL_SIZE:  " << QString::number(size()) << '\n';
//            os << "FILE_LIST:   " << file_list.join(";") << '\n';
//            os << "FOLDER_LIST: " << folder_list.join(";") << '\n';
//            return os;
//        }
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

    /**
     * @brief resetWindowColor 重置ui颜色，在获取完毕处理完毕后可以调用
     */
    inline void resetWindowColor() {_ui_normal();}

    ~DraggerWidget();
    /*****************************接口*****************************/

private:
    Ui::DraggerWidget *ui;
    DragObjects_t _last_d;

private:
    /**
     * @brief _initWidget 初始化窗口
     */
    void _initWidget();
    struct _Color {
        int r; int g; int b;
    };
    inline void _setBackground(int r, int g, int b) {this->setStyleSheet(QString("QWidget{background-color:rgb(%0,%1,%2)}").arg(r).arg(g).arg(b));}
    inline void _setBackground(const _Color& c) {_setBackground(c.r, c.g, c.b);}
    inline void _yellow()   {this->setStyleSheet(QString("QWidget{background-color:yellow}"));}
    inline void _green()    {this->setStyleSheet(QString("QWidget{background-color:green}"));}
    inline void _red()      {this->setStyleSheet(QString("QWidget{background-color:red}"));}
    inline void _orange()   {this->setStyleSheet(QString("QWidget{background-color:orange}"));}

    /**
     * @brief _ui_success 可以在这里自定义一些ui的行为
     */
    void _ui_success();
    void _ui_fail();
    void _ui_dragging();
    void _ui_normal();

private:
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
