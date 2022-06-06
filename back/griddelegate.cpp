#include "griddelegate.h"
#include "back.h"
GridDelegate::GridDelegate(size_t widthDeclare, size_t heightDeclare, QObject *parent)
    : QAbstractListModel {parent},
      m_width {widthDeclare},
      m_height {heightDeclare},
      gridSize {heightDeclare * widthDeclare}
{
    m_RawBoard.resize(gridSize);
}

int GridDelegate::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_RawBoard.size();
}

QVariant GridDelegate::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || role !=Qt::DisplayRole){
        return {};
    }
    const int rowIndex {index.row()};
    return QVariant::fromValue(m_RawBoard[rowIndex].id);
}


int GridDelegate::height() const
{
    return m_height;
}

void GridDelegate::setHeight(size_t newHeight)
{
    if (m_height == newHeight)
        return;
    m_height = newHeight;
    emit heightChanged();
}

int GridDelegate::width() const
{
    return m_width;
}

void GridDelegate::setWidth(size_t newWidth)
{
    if (m_width == newWidth)
        return;
    m_width = newWidth;
    emit widthChanged();
}

///
/// \brief GridDelegate::updateModel
/// список  вершин
///   Вектор айди + имя
/// список ребер
///
/// размер таблицы
/// сама таблица
///

void GridDelegate::setType(int i, int j, Schema *print, std::vector<int> *path)
{
    for(uint ii = 0;ii<m_height;ii++)
        for(uint jj = 0;jj<m_width;jj++)
            if (print->schema_map[ii][jj]==print->schema_map[i][j])
                path->push_back(ii*m_width+jj);
}

void GridDelegate::resizeGrid(int width, int height)
{
    m_RawBoard.resize(width * height);
    m_width = width;
    m_height = height;
}




