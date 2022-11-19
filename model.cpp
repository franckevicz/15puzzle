#include "model.h"
#include <QQmlEngine>

#include <algorithm>
#include <random>
#include <chrono>

namespace {

} //anonymous namespace

GameModel::GameModel(QObject* parent/* = 0*/)
    : QAbstractListModel(parent)
{
    newGame();
    _dataChanged();
}

GameModel::~GameModel() {}

bool GameModel::canPress(int val) {
    if (val < 0 || val >= rowCount())
        return false;

    auto it = std::find(std::begin(cells), std::end(cells), val);
    auto idx = it - std::begin(cells);

    auto neibour_indexes = _getNearCellsIndexes(idx);
    for (auto& neibour_idx: neibour_indexes) {
        if (cells[neibour_idx] == 0)
            return true;
    }

    return false;
}

bool GameModel::isGameFinished() const
{
    for (auto i = 0; i < (rowCount() - 1); ++ i) {
        if (cells[i] != (i + 1))
            return false;
    }

    return true;
}

std::vector<int> GameModel::_getNearCellsIndexes(int idx) const
{
    std::vector<int> res;
    static std::vector<std::pair<int, int>> shifts = {{-1, 0},
                                                      {1, 0},
                                                      {0, -1},
                                                      {0, 1}};

    auto initial_row_cols_sum = _getRow(idx) + _getCol(idx);

    for (auto& shift: shifts) {
        int candidate = idx + (shift.first * m_boardsize)  + shift.second;
        auto candidate_row_cols_sum = _getRow(candidate) + _getCol(candidate);
        auto row_cols_difference = std::abs(initial_row_cols_sum - candidate_row_cols_sum);
        if (candidate >= 0 && candidate < rowCount() && row_cols_difference == 1)
            res.emplace_back(candidate);
    }

    return res;
}

void GameModel::_dataChanged() {
    emit dataChanged(createIndex(0, 0), createIndex((m_boardsize * m_boardsize - 1), 0));
    emit gameFinishedChanged();
}

int GameModel::_getRow(int idx) const
{
    return idx % m_boardsize;
}

int GameModel::_getCol(int idx) const
{
    return idx / m_boardsize;
}

void GameModel::shuffle() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    do {
    std::shuffle(std::begin(cells), std::end(cells), std::default_random_engine(seed++));
    } while (_isSolvable());

    cached_cells = cells;
    m_game_finished = false;
    _dataChanged();
}

void GameModel::reset() {
    if (cells == cached_cells)
        return ;

     cells = cached_cells;
     m_game_finished = false;
     _dataChanged();
}

void GameModel::newGame()
{
    auto new_size = m_boardsize * m_boardsize;
    auto old_size = cells.size();
    if (new_size != old_size)
    {
    bool increasing_cells = new_size > old_size;

    if (increasing_cells)
       beginInsertRows(QModelIndex(), old_size, new_size - 1);
    else
       beginRemoveRows(QModelIndex(), new_size, old_size - 1);

    cells.resize(new_size);
    std::iota(std::begin(cells), std::end(cells), 0);

    if (increasing_cells)
       endInsertRows();
    else
       endRemoveRows();
    }

    shuffle();
}

void GameModel::swapItemWithZero(int idx) {
    if (idx < 0 || idx >= rowCount())
        return;

    if (!canPress(cells[idx]))
        return;

    auto zero_idx = getHiddenIndex();
    std::swap(cells[idx], cells[zero_idx]);
    _dataChanged();
}

QVariant GameModel::data(const QModelIndex &index, int role) const {
    return {cells[index.row()]};
}

bool GameModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (cells[index.row()] == value.toInt())
        return false;

    cells[index.row()] = value.toInt();
    return true;
}

bool GameModel::_isSolvable() const
{
    int inversions = 0;
    for (int i = 0; i < m_boardsize * m_boardsize - 1; i++)
    {
        for (int j = i + 1; j < m_boardsize * m_boardsize; j++)
        {
            if (cells[j] && cells[i] && cells[i] > cells[j])
                inversions++;
        }
    }

    bool is_inversions_even = inversions % 2 != 0;
    // if m_boardsize is odd board in solvable with even inversions
    if (m_boardsize % 2 != 0)
    {
        return !is_inversions_even;
    }
    else //if m_boardsize is even
    {
        auto row_of_zero = _getRow(getHiddenIndex());
        bool row_odd_inv_even = row_of_zero % 2 != 0 && is_inversions_even;
        bool row_even_inv_odd = row_of_zero % 2 == 0 && !is_inversions_even;

        return row_odd_inv_even || row_even_inv_odd;
    }
}

int GameModel::rowCount(const QModelIndex &parent) const {
    return cells.size();
}

int GameModel::getHiddenIndex() const {
    auto zero_it = std::find(std::begin(cells), std::end(cells), 0);
    return zero_it - std::begin(cells);
}

uint8_t GameModel::boardsize() const
{
    return m_boardsize;
}

void GameModel::setBoardsize(uint8_t newBoardsize)
{
    if (newBoardsize < 2)
        return;

    if (m_boardsize == newBoardsize)
        return;

    m_boardsize = newBoardsize;
    emit boardsizeChanged();
}
