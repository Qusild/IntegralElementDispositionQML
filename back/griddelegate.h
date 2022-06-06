#ifndef GRIDDELEGATE_H
#define GRIDDELEGATE_H

#include <QObject>
#include <QAbstractListModel>
#include <string>
#include <vector>
#include "back.h"
class GridDelegate : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
public:
    static constexpr size_t defaultWidth = 10;
    static constexpr size_t defaultHeight = 10;
    GridDelegate(size_t widthDeclare = defaultWidth, size_t heightDeclare = defaultHeight,
                    QObject* parent = nullptr);

    struct Tile{
        int id = 0;
        int type = 0;
        std::string name;
        std::vector<int> connections;
    };

    int rowCount(const QModelIndex& parent = QModelIndex{}) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    int height() const;
    void setHeight(size_t newHeightSet);

    int width() const;
    void setWidth(size_t newWidth);
    void setType(int i, int j, Schema *print,std::vector<int> *path);
signals:
    void getSize(int widthRet,int heightRet);
    void heightChanged();
    void widthChanged();
public slots:
    void resizeGrid(int width, int heigth);

private:
    std::vector<Tile> m_RawBoard;
    size_t m_width;
    size_t m_height;
    size_t gridSize;

};

#endif // GRIDDELEGATE_H
