#include "Matchmaking.h"
#include "iostream"

Matchmaking::Matchmaking()
    : cardHolder("../../static/cards/")
{
}

void Matchmaking::findMath(crow::websocket::connection* conn, const std::string viewer_id)

{
    std::cout << "Try to find game" << std::endl;
    Match match(2, cardHolder);
    int maxSize = match.getMaxSize();
    players.insert({ conn, std::make_shared<Player>(conn, viewer_id) });

    std::lock_guard<std::mutex> guard(find);

    queue.push_back(players[conn]);

    if (queue.size() >= maxSize) {
        auto iterator = queue.begin();
        auto size = queue.size();
        int counter = 0;

        for (int i = 0; i < size && counter < maxSize; i++) {
            std::cout << "||Queue work...\n";
            if (iterator->lock()) {
                match.addPlayer(iterator->lock());
                counter++;

                std::cout<<"\nCan lock pointer!\n";
                std::cout<<iterator->lock()->getViewer_id();
            }
            queue.erase(iterator);

        }
        if (counter < maxSize) {
            auto returningPlayers = match.getPlayers();
            for (auto& player : returningPlayers) {
                queue.push_back(player);
            }
            std::cout << "\n Cant find game!\n";
        } else {
            std::cout << "\nFind game!\n";
            auto gamers = match.getPlayers();
            for (auto& gamer : gamers) {
                matches.insert({ gamer->getConn(), match });
            }

            std::cout << "\nMatch created!\n";
            sendNotifyStartGame(match);
        }
    }
}

void Matchmaking::removePlayer(crow::websocket::connection* conn)
{
    players.erase(conn);
}

void Matchmaking::sendNotifyStartGame( Match& match)
{
    std::vector<std::string> response = parser.createMatch(match);
    const std::vector<PlayerSP> & players = match.getPlayers();
    for (int i = 0; i < players.size(); i++) {
        std::cout<<"\n Response\n"<<response[i];
        players[i]->getConn()->send_text(response[i]);

    }
}
