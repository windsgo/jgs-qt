#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "myjgs.h"
#include <QTextCodec>
#include <QVBoxLayout>
#include <QTextStream>
#include <QDir>
#include <QDateTime>
#include <QElapsedTimer>
#include <QFileDialog>
#include <locale>

static QStringList find_file(const QString& start_dir, const QStringList& filters);

MainWindow::MainWindow(QWidget *parent)
    : DraggerWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, &DraggerWidget::sthDragged, this, [&](const DraggerWidget::DragObjects_t& d) {
        ui->textBrowser->clear();
        _jgs_file_list.clear();

        QString default_save_path;

        qDebug() << d;

        switch (d.type) {
        case TYPE_NOTHING:{
            return;
        }
        case TYPE_FILE:{
            default_save_path = QDir::cleanPath(d.file_list.at(0));
            for (auto& file : d.file_list) {
                _check_jgs_and_insert(file);
            }
            break;
        }
        case TYPE_FOLDER:{
            default_save_path = QDir::cleanPath(d.folder_list.at(0));
            for (auto& folder : d.folder_list) {
                auto files = find_file(folder, QStringList({"*.jgs"}));
                for (auto& file : files) {
                    _check_jgs_and_insert(file);
                }
            }
            break;
        }
        case TYPE_MIX:{
            default_save_path = QDir::cleanPath(d.file_list.at(0));
            auto files = find_file(QFileInfo(d.file_list.at(0)).path(), QStringList({"*.jgs"}));
            for (auto& file : files) {
                _check_jgs_and_insert(file);
            }
            break;
        }
        default:
            Q_ASSERT(false);
            return;
        }

        auto ui_path = ui->le_savePath->text();
        QFileInfo ui_path_info(ui_path);
        if (!ui_path_info.isDir()) {
            qDebug() << "!ui_path_info.isDir()";
            ui->le_savePath->setText(QDir::toNativeSeparators(QFileInfo(default_save_path).path()));
        }

        QString save_file_name = ui->le_savePath->text() + QDir::separator()
                + QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss") + ".txt";
        save_file_name = QDir::toNativeSeparators(save_file_name);
        qDebug() << save_file_name;
        QFile file(save_file_name);
        if (!file.open(QIODevice::OpenModeFlag::ReadWrite)) {
            ui->textBrowser->setText("save file failed");
            return;
        }

        if (_jgs_file_list.empty()) {
            ts.reset();
            file.close();
            return;
        }

        ts.setDevice(&file);

        int successful_process_num = 0; // 记录成功处理的文件数目

        for (auto& jgs_file : _jgs_file_list) {
//            qDebug() << jgs_file;
            if (_handle_one_file(QDir::toNativeSeparators(jgs_file))) ++successful_process_num;
        }

        ui->textBrowser->append(QString("successful handle %0 files").arg(successful_process_num));
        ui->textBrowser->append(QString("saved to %0").arg(save_file_name));

        ts.flush();
        ts.reset();
        file.close();

    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::_check_jgs_and_insert(const QString &filename)
{
    QFileInfo info(filename);
    if (info.suffix().toLower() == "jgs") {
        _jgs_file_list.append(filename);
    }
}

bool MainWindow::_handle_one_file(const QString &filename)
{
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
    QTextCodec *gbk = QTextCodec::codecForName("gb2312");
    QTextCodec::setCodecForLocale(utf8);

    ts << "-----------START------------\n";
    ts << "Processing:" + filename + "\n\n";
    ui->textBrowser->append("processing:" + QFileInfo(filename).fileName());

    bool ret = false;

    static auto color_sequence = std::array<myjgs::PlayerColor, 4> {
        myjgs::PlayerColor::YELLOW,
        myjgs::PlayerColor::PURPLE,
        myjgs::PlayerColor::GREEN,
        myjgs::PlayerColor::BLUE,
    };

    try {

        myjgs::Game::ptr game = std::make_shared<myjgs::Game>(filename.toStdString());
        game->process_all_events();

        for (uint8_t i = 0; i < 4; ++i) {
            // myjgs::PlayerColor color = (myjgs::PlayerColor)i;
            auto color = color_sequence[i];

            ts << ("color:" + QString(myjgs::itemcolor_type2string(color).c_str()) + "\n");
            ts << ("qq:" + QString::number(game->get_player(color).qq()) + "\n");
            QString name_unicode = gbk->toUnicode(game->get_player(color).name());
            QByteArray name_utf8 = utf8->fromUnicode(name_unicode);
            ts << ("name:" + name_utf8 + "\n");
            ts << ("remain socre:" + QString::number(game->get_player(color).get_current_score()) + "\n");

            auto player_stat = game->get_player(color).get_stat();
            ts << QString{"神兵天降: 工兵击杀值: %0"}.arg(player_stat[0]) << '\n';
            ts << QString{"拆弹专家: 击杀炸弹数: %0"}.arg(player_stat[1]) << '\n';
            ts << QString{"运筹帷幄: 司令击杀值: %0"}.arg(player_stat[2]) << '\n';
            ts << QString{"七、八大天王: 403938击杀数: %0"}.arg(player_stat[3]) << "\n\n";
        }

        ret = true;

    } catch (const myjgs::GameException& ge) {
        QString tmp{"GameException: "};
        tmp.append(ge.what());
        ui->textBrowser->append(tmp);
    } catch (const std::exception& stde) {
        QString tmp{"std::exception: "};
        tmp.append(stde.what());
        ui->textBrowser->append(tmp);
    } catch (...) {
        ui->textBrowser->append("unknown error");
    }

    ui->textBrowser->append("process over:" + QFileInfo(filename).fileName());
    ts << "-----------END------------\n";
    return ret;
}

static QStringList find_file(const QString &start_dir, const QStringList &filters)
{
    QStringList names;

    if (!QFileInfo(start_dir).isDir()) return names;

    QDir dir(start_dir);

    for(auto& file : dir.entryList(filters, QDir::Files)) {
        names.append(start_dir + QDir::separator() + file);
    }

    for(auto& subdir : dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot)) {
        names.append(find_file(start_dir + QDir::separator() + subdir, filters));
    }

    return names;
}


void MainWindow::on_pb_choosepath_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("select dir to save"), QString(),
                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        ui->le_savePath->setText(dir);
    }
}
