#pragma once
#include <QAbstractListModel>
#include <QQmlContext>
#include <QArrayData>

class GameModel : public QAbstractListModel
{
    Q_OBJECT

public:
    GameModel(QObject* parent = 0);
    virtual ~GameModel();

    enum TileRoles {
        ValueRole = Qt::UserRole + 1
    };

    QHash<int, QByteArray> roleNames() const override {
        static QHash<int, QByteArray> roles = {
            {ValueRole, "value"}};
        return roles;
    }

    Q_INVOKABLE bool canPress(int val);

    Q_INVOKABLE void reset();
    Q_INVOKABLE void newGame();

    Q_INVOKABLE void swapItemWithZero(int idx);

    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE int getHiddenIndex() const;

    Q_PROPERTY(uint8_t boardsize READ boardsize WRITE setBoardsize NOTIFY boardsizeChanged)
    Q_PROPERTY(bool gameFinished  READ isGameFinished NOTIFY gameFinishedChanged)

    bool isGameFinished() const;

    uint8_t boardsize() const;
    void setBoardsize(uint8_t newBoardsize);

signals:
    void boardsizeChanged();
    void gameFinishedChanged();

private:
    Q_INVOKABLE virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void shuffle();
    bool _isSolvable() const;
    std::vector<int> _getNearCellsIndexes(int idx) const;
    void _dataChanged();
    int _getRow(int idx) const;
    int _getCol(int idx) const;

private:
    bool m_game_finished = false;
    uint8_t m_boardsize = 4;
    QVector<int> cells;
    QVector<int> cached_cells;
};
