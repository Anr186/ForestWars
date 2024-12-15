#include <iostream>
#include <conio.h>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <sstream>

using namespace std;
using namespace sf;

queue<string> messageQueue;

void addMessage(const string& message) {
    messageQueue.push(message);
    if (messageQueue.size() > 5) {
        messageQueue.pop();
    }
}

class Model {
private:
    int N;
    int** array;

public:
    Model(int N) : N(N) {
        array = new int* [N];
        for (int i = 0; i < N; ++i) {
            array[i] = new int[N]();
        }
    }

    ~Model() {
        for (int i = 0; i < N; ++i) {
            delete[] array[i];
        }
        delete[] array;
    }

    int** getArray() {
        return array;
    }

    int getN() const {
        return N;
    }

    void printMap() const {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cout << array[i][j] << " ";
            }
            cout << endl;
        }
    }
};

class Animal {
protected:
    int x;
    int y;
    int age;
    string name;

public:
    Animal(int x, int y, int age, const string& name) : x(x), y(y), age(age), name(name) {}

    virtual int getDirection() const = 0;
    virtual int getAge() const = 0;
    virtual void setAge(int newAge) = 0;
    virtual void setPosAnimal(Model& map1, int ch) = 0;
    virtual void setFirstPosAnimal(int newX, int newY, Model& map1) = 0;
    virtual void deathFromTime(int x, int y, Model& map1) = 0;

    const string& getName() const {
        return name;
    }

    void printCoords() const {
        cout << "X: " << x << "\tY: " << y << endl;
    }

    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }
};

class Fox : public Animal {
private:
    int direction;

public:
    Fox(int x, int y, int age, const string& name, int direction) : Animal(x, y, age, name), direction(direction) {}

    ~Fox() {
        cout << getName() << " left the server." << endl;
    }

    int getDirection() const override {
        return direction;
    }

    int getAge() const override {
        return age;
    }

    void setAge(int newAge) override {
        age = newAge;
    }

    void setFirstPosAnimal(int newX, int newY, Model& map1) override {
        map1.getArray()[x][y] = 0;
        map1.getArray()[newX][newY] = 2;
        x = newX;
        y = newY;
    }

    void setPosAnimal(Model& map1, int ch) override {
        int** array = map1.getArray();
        int N = map1.getN();
        array[x][y] = 0;

        switch (ch) {
        case 72: x = (x - 2 + N) % N; break;
        case 80: x = (x + 2) % N; break;
        case 75: y = (y - 2 + N) % N; break;
        case 77: y = (y + 2) % N; break;
        }
        array[x][y] = 2;
    }

    void deathFromTime(int x, int y, Model& map1) override {
        map1.getArray()[x][y] = 0;
    }
};

class Krol : public Animal {
private:
    int direction;

public:
    Krol(int x, int y, int age, const string& name, int direction) : Animal(x, y, age, name), direction(direction) {}

    ~Krol() {
        cout << getName() << " left the server." << endl;
    }

    int getDirection() const override {
        return direction;
    }

    int getAge() const override {
        return age;
    }

    void setAge(int newAge) override {
        age = newAge;
    }

    void setFirstPosAnimal(int newX, int newY, Model& map1) override {
        map1.getArray()[x][y] = 0;
        map1.getArray()[newX][newY] = 1;
        x = newX;
        y = newY;
    }

    void setPosAnimal(Model& map1, int ch) override {
        int** array = map1.getArray();
        int N = map1.getN();
        array[x][y] = 0;

        int newX = x;
        int newY = y;

        switch (ch) {
        case 72: newX = (x - 1 + N) % N; break; // UP
        case 80: newX = (x + 1) % N; break;      // DOWN
        case 75: newY = (y - 1 + N) % N; break;  // LEFT
        case 77: newY = (y + 1) % N; break;      // RIGHT
        }

        if (array[newX][newY] == 0) {
            x = newX;
            y = newY;
            array[x][y] = 1;
        }
        else {
            array[x][y] = 1;
        }
    }

    bool Reproduce(vector<Krol*>& masR, Model& map1) {
        if (age % 5 == 0) {
            int dx = rand() % 3 - 1;
            int dy = rand() % 3 - 1;
            int newX = (x + dx + map1.getN()) % map1.getN();
            int newY = (y + dy + map1.getN()) % map1.getN();

            if (map1.getArray()[newX][newY] == 0) {
                string newName = "Krol_baby" + to_string(masR.size() + 1);
                auto newKrol = new Krol(newX, newY, 0, newName, direction);
                newKrol->setFirstPosAnimal(newX, newY, map1);
                masR.push_back(newKrol);
                cout << getName() << " gave birth to " << newName << " at (" << newX << ", " << newY << ")" << endl;
                return true;
            }
        }
        return false;
    }

    void DeathFromFox(int x, int y, Model& map1)
    {
        int** array = map1.getArray();
        array[x][y] = 2;
    }

    void deathFromTime(int x, int y, Model& map1) override {
        map1.getArray()[x][y] = 0;
    }
};

class Bear : public Animal {
public:
    Bear(int x, int y, int age, const string& name) : Animal(x, y, age, name) {}

    ~Bear() {
        cout << getName() << " left the server." << endl;
    }

    int getDirection() const override {
        return 0;
    }

    int getAge() const override {
        return age;
    }

    void setAge(int newAge) override {
        age = newAge;
    }

    void setFirstPosAnimal(int newX, int newY, Model& map1) override {
        map1.getArray()[x][y] = 0;
        map1.getArray()[newX][newY] = 3;
        x = newX;
        y = newY;
    }

    void setPosAnimal(Model& map1, int ch) override {
        int** array = map1.getArray();
        int N = map1.getN();
        array[x][y] = 0;

        int dx = rand() % 3 - 1; // -1, 0, or 1
        int dy = rand() % 3 - 1; // -1, 0, or 1

        int newX = (x + dx + N) % N;
        int newY = (y + dy + N) % N;

        x = newX;
        y = newY;
        array[x][y] = 3;
    }

    void deathFromTime(int x, int y, Model& map1) override {
        map1.getArray()[x][y] = 0;
    }

    bool eatNearbyAnimals(vector<Krol*>& masR, vector<Fox*>& masF, Model& map1) {
        int N = map1.getN();
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                int checkX = (x + dx + N) % N;
                int checkY = (y + dy + N) % N;

                for (auto it = masR.begin(); it != masR.end(); ) {
                    if ((*it)->getX() == checkX && (*it)->getY() == checkY) {
                        addMessage(getName() + " ate " + (*it)->getName() + " at (" + to_string(checkX) + ", " + to_string(checkY) + ")");
                        (*it)->deathFromTime(checkX, checkY, map1);
                        it = masR.erase(it);
                        return true;
                    }
                    else {
                        ++it;
                    }
                }

                for (auto it = masF.begin(); it != masF.end(); ) {
                    if ((*it)->getX() == checkX && (*it)->getY() == checkY) {
                        addMessage(getName() + " ate " + (*it)->getName() + " at (" + to_string(checkX) + ", " + to_string(checkY) + ")");
                        (*it)->deathFromTime(checkX, checkY, map1);
                        it = masF.erase(it);
                        return true;
                    }
                    else {
                        ++it;
                    }
                }
            }
        }
        return false;
    }
};

class Game {
private:
    RenderWindow& window;
    Model& map1;
    int n;
    vector<Krol*>& masR;
    vector<Fox*>& masF;
    vector<Bear*>& masB;

public:
    Game(RenderWindow& window, Model& map1, int n, vector<Krol*>& masR, vector<Fox*>& masF, vector<Bear*>& masB)
        : window(window), map1(map1), n(n), masR(masR), masF(masF), masB(masB) {}

    void drawFirstScreen(int& flag_window_type) {
        Texture mainmenu_background;
        mainmenu_background.loadFromFile("misc/main_menu.png");

        Sprite mainmenu_background_sprite(mainmenu_background);
        mainmenu_background_sprite.setPosition(0, 0);

        Font font;
        font.loadFromFile("misc/roboto.ttf");

        RectangleShape button_1_mm(Vector2f(300.f, 100.f));
        button_1_mm.setFillColor(Color(107, 158, 53, 0));
        button_1_mm.setPosition(690, 660);

        Text playText("Play", font, 70);
        playText.setFillColor(Color(42, 176, 28));
        playText.setOutlineColor(Color::Black);
        playText.setOutlineThickness(3.f);
        playText.setPosition(button_1_mm.getPosition().x + (button_1_mm.getSize().x - playText.getGlobalBounds().width) / 2, button_1_mm.getPosition().y + (button_1_mm.getSize().y - playText.getGlobalBounds().height) / 2 - 10);

        Event event1;
        while (window.isOpen()) {
            window.clear();
            while (window.pollEvent(event1)) {
                if (event1.type == Event::Closed) {
                    window.close();
                }

                if (button_1_mm.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                    button_1_mm.setFillColor(Color(60, 179, 255, 0));
                    if (event1.type == Event::MouseButtonPressed && event1.mouseButton.button == Mouse::Left) {
                        flag_window_type = 1;
                    }
                }
                else {
                    button_1_mm.setFillColor(Color(46, 139, 87, 0));
                }
            }
            window.draw(mainmenu_background_sprite);
            window.draw(button_1_mm);
            window.draw(playText);
            window.display();
            if (flag_window_type == 1)break;
        }
    }

    void drawSecondScreen(int& flag_window_type) {
        int number_of_krols = 0;
        int isPassEnter = -1;
        int dy_stackinput_m2 = 0;
        vector<string> array_stack_input;
        string input_str_m2;
        int startXfield = 650;
        int startYfield = 300;

        int field_scale = 54;//27px

        int** array = map1.getArray();

        Font font;
        font.loadFromFile("misc/roboto.ttf");

        Text title("Forest Wars", font, 100);
        title.setPosition(600, 30);
        title.setFillColor(Color::White);
        title.setOutlineColor(Color::Black);
        title.setOutlineThickness(5.f);

        Texture menu2_background;
        menu2_background.loadFromFile("misc/menu3.png");
        Sprite menu2_background_sprite(menu2_background);
        menu2_background_sprite.setPosition(0, 0);

        Texture fieldtexture_m3;
        fieldtexture_m3.loadFromFile("misc/net.png", IntRect(0, 0, 27, 27));
        fieldtexture_m3.setRepeated(true);

        Texture icons;
        icons.loadFromFile("misc/icons.png");

        Texture lil_icons;
        lil_icons.loadFromFile("misc/icons_lil.png");

        Sprite icon_fox(icons, IntRect(50, 50, 450, 450));
        icon_fox.setScale(0.2f, 0.2f);
        icon_fox.setPosition(110.f, 300.f);

        Sprite icon_rob(icons, IntRect(500, 500, 480, 480));
        icon_rob.setScale(0.2f, 0.2f);
        icon_rob.setPosition(210.f, 300.f);

        Sprite icon_bear(icons, IntRect(500, 50, 450, 450));
        icon_bear.setScale(0.2f, 0.2f);
        icon_bear.setPosition(310.f, 300.f);

        Sprite small_fox(lil_icons, IntRect(50, 50, 450, 450));
        small_fox.setScale(0.12f, 0.12f);
        Sprite small_rob(lil_icons, IntRect(500, 500, 480, 480));
        small_rob.setScale(0.12f, 0.12f);
        Sprite small_bear(lil_icons, IntRect(500, 50, 450, 450));
        small_bear.setScale(0.12f, 0.12f);

        bool gameStarted = false;

        RectangleShape startButton(Vector2f(200, 100));
        startButton.setFillColor(Color(0, 0, 0, 0));
        startButton.setPosition(100, 400);

        Text startButtonText("Start", font, 60);
        startButtonText.setFillColor(Color::White);
        startButtonText.setOutlineColor(Color::Black);
        startButtonText.setOutlineThickness(3.f);
        startButtonText.setPosition(
            startButton.getPosition().x + (startButton.getSize().x - startButtonText.getGlobalBounds().width) / 2,
            startButton.getPosition().y + (startButton.getSize().y - startButtonText.getGlobalBounds().height) / 2 - 10
        );

        Sprite field_m3(fieldtexture_m3, IntRect(0, 0, 216, 216));
        field_m3.setColor(Color(123, 232, 161, 70));
        field_m3.setScale(2.f, 2.f);
        field_m3.setPosition(startXfield, startYfield);

        RectangleShape button_1_mm(Vector2f(150.f, 75.f));
        button_1_mm.setFillColor(Color::Red);
        button_1_mm.setPosition(870, 450);

        RectangleShape matrixBorder(Vector2f(n * field_scale - 4, n * field_scale - 4));
        matrixBorder.setFillColor(Color::Transparent);
        matrixBorder.setOutlineThickness(5);
        matrixBorder.setOutlineColor(Color(56, 143, 86));
        matrixBorder.setPosition(startXfield, startYfield);

        vector<vector<RectangleShape>> a(n, vector<RectangleShape>(n));
        RectangleShape net(Vector2f(field_scale, field_scale));

        bool placingFox = false;
        bool placingRabbit = false;
        bool placingBear = false;

        Event event2;
        while (window.isOpen()) {
            while (window.pollEvent(event2)) {
                if (event2.type == Event::Closed) {
                    window.close();
                }

                if (!gameStarted && startButton.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                    startButton.setFillColor(Color(0, 0, 0, 0));
                    if (event2.type == Event::MouseButtonPressed && event2.mouseButton.button == Mouse::Left) {
                        gameStarted = true;
                        placingFox = false;
                        placingRabbit = false;
                        placingBear = false;
                        flag_window_type = 2;
                    }
                }
                else {
                    startButton.setFillColor(Color(0, 0, 0, 0));
                }

                if (icon_fox.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                    icon_fox.setColor(Color(255, 255, 255, 150));
                    if (event2.type == Event::MouseButtonPressed) {
                        placingFox = true;
                        placingRabbit = false;
                        placingBear = false;
                    }
                }
                else {
                    icon_fox.setColor(Color(255, 255, 255, 255));
                }

                if (icon_rob.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                    icon_rob.setColor(Color(255, 255, 255, 150));
                    if (event2.type == Event::MouseButtonPressed) {
                        placingRabbit = true;
                        placingFox = false;
                        placingBear = false;
                    }
                }
                else {
                    icon_rob.setColor(Color(255, 255, 255, 255));
                }

                if (icon_bear.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                    icon_bear.setColor(Color(255, 255, 255, 150));
                    if (event2.type == Event::MouseButtonPressed) {
                        placingBear = true;
                        placingFox = false;
                        placingRabbit = false;
                    }
                }
                else {
                    icon_bear.setColor(Color(255, 255, 255, 255));
                }

                if (event2.type == Event::MouseButtonPressed) {
                    for (int i = 0; i < n; i++) {
                        for (int j = 0; j < n; j++) {
                            if (a[i][j].getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                                if (!gameStarted) {
                                    if (placingFox) {
                                        map1.getArray()[i][j] = 2;
                                        placingFox = false;
                                    }
                                    else if (placingRabbit) {
                                        map1.getArray()[i][j] = 1;
                                        placingRabbit = false;
                                    }
                                    else if (placingBear) {
                                        map1.getArray()[i][j] = 3;
                                        placingBear = false;
                                    }
                                    else {
                                        map1.getArray()[i][j] = 0;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            window.clear();
            window.draw(menu2_background_sprite);
            window.draw(icon_fox);
            window.draw(icon_rob);
            window.draw(icon_bear);
            window.draw(title);
            window.draw(matrixBorder);

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    a[j][i] = net;
                    a[j][i].setPosition(startXfield + i * field_scale, startYfield + j * field_scale);

                    if (map1.getArray()[j][i] == 1) {
                        a[j][i].setFillColor(Color(128, 128, 128, 180));
                        small_rob.setPosition(startXfield + i * field_scale, startYfield + j * field_scale);
                        window.draw(a[j][i]);
                        window.draw(small_rob);
                    }
                    else if (map1.getArray()[j][i] == 2) {
                        a[j][i].setFillColor(Color(255, 0, 0, 180));
                        small_fox.setPosition(startXfield + i * field_scale, startYfield + j * field_scale);
                        window.draw(a[j][i]);
                        window.draw(small_fox);
                    }
                    else if (map1.getArray()[j][i] == 3) {
                        a[j][i].setFillColor(Color(139, 69, 19, 180));
                        small_bear.setPosition(startXfield + i * field_scale, startYfield + j * field_scale);
                        window.draw(a[j][i]);
                        window.draw(small_bear);
                    }
                    else {
                        if (a[j][i].getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                            a[j][i].setFillColor(Color::Yellow);
                        }
                        else {
                            a[j][i].setFillColor(Color(0, 90, 0, 200));
                        }
                        window.draw(a[j][i]);
                    }
                }
            }
            window.draw(field_m3);
            window.draw(matrixBorder);
            if (!gameStarted) {
                window.draw(startButton);
                window.draw(startButtonText);
            }
            else
            {
                break;
            }
            window.display();
        }
    }

    void drawThirdScreen(int& flag_window_type) {
        Music music;
        music.openFromFile("misc/combat01.mp3");
        music.setVolume(60);
        music.setLoop(true);  
        music.play();
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (map1.getArray()[i][j] == 1)
                {
                    string name = "Krol_player" + to_string(masR.size() + 1);
                    auto krol = new Krol(i, j, 0, name, 0);
                    masR.push_back(krol);
                }
                else if (map1.getArray()[i][j] == 2)
                {
                    string name = "Fox_player" + to_string(masF.size() + 1);
                    auto fox = new Fox(i, j, 0, name, 0);
                    masF.push_back(fox);
                }
                else if (map1.getArray()[i][j] == 3)
                {
                    string name = "Bear_player" + to_string(masB.size() + 1);
                    auto bear = new Bear(i, j, 0, name);
                    masB.push_back(bear);
                }
            }
        }
        cout << "Start position" << endl;
        map1.printMap();

        int ch = 0;
        int step = 0;
        int moveCount = 0;

        int** array = map1.getArray();
        int field_scale = 54;
        int startXfield = 650;
        int startYfield = 300;

        Texture lil_icons;
        lil_icons.loadFromFile("misc/icons_lil.png");
        Texture fieldtexture_m3;
        fieldtexture_m3.loadFromFile("misc/net.png", IntRect(0, 0, 27, 27));
        fieldtexture_m3.setRepeated(true);

        RectangleShape button_1_mm(Vector2f(300.f, 100.f));
        button_1_mm.setFillColor(Color(200, 1, 1, 180));
        button_1_mm.setPosition(1450, 800);

        RectangleShape matrixBorder(Vector2f(n * field_scale - 4, n * field_scale - 4));
        matrixBorder.setFillColor(Color::Transparent);
        matrixBorder.setOutlineThickness(5);
        matrixBorder.setOutlineColor(Color(56, 143, 86));
        matrixBorder.setPosition(startXfield, startYfield);

        Sprite small_fox(lil_icons, IntRect(50, 50, 450, 450));
        small_fox.setScale(0.12f, 0.12f);
        Sprite small_rob(lil_icons, IntRect(500, 500, 480, 480));
        small_rob.setScale(0.12f, 0.12f);

        Sprite small_bear(lil_icons, IntRect(500, 50, 450, 450));
        small_bear.setScale(0.12f, 0.12f);

        Sprite field_m3(fieldtexture_m3, IntRect(0, 0, 216, 216));
        field_m3.setColor(Color(123, 232, 161, 70));
        field_m3.setScale(2.f, 2.f);
        field_m3.setPosition(startXfield, startYfield);

        Font font;
        font.loadFromFile("misc/roboto.ttf");
        Text title("Forest Wars", font, 100);
        title.setPosition(600, 30);
        title.setFillColor(Color::Green);
        title.setOutlineColor(Color::Black);
        title.setOutlineThickness(5.f);

        Text rulesTitle(L"Правила", font, 40);
        rulesTitle.setPosition(100, 240); 
        rulesTitle.setFillColor(Color::White);
        rulesTitle.setOutlineColor(Color::Black);
        rulesTitle.setOutlineThickness(2.f);

        RectangleShape rulesBackground(Vector2f(350, 400));
        rulesBackground.setFillColor(Color(34, 112, 43, 200));
        rulesBackground.setPosition(50, 300);

        wstring rules_text =
            L"Заяц -> Лиса -> Медведь\n"
            L"Медведь - ИИ OpenAI \n\n"
            L"На сколько клеток ходит:\n"
            L"    - лиса - 2\n"
            L"    - заяц - 1\n"
            L"    - медведь - 2\n"
            L"Радиус атаки:\n"
            L"    - лиса - 1\n"
            L"    - заяц пацифист\n"
            L"    - медведь - 2\n"
            L"Время жизни:\n"
            L"    - лиса - 15\n"
            L"    - заяц - 10\n"
            L"    - медведь - 20\n"
            L"Условия размножения:\n"
            L"    - заяц - каждые 5 лет\n";
        
        Text rules(rules_text, font, 20);
        rules.setPosition(50, 300);
        rules.setFillColor(Color(150, 214, 182));
        rules.setOutlineColor(Color::Black);
        rules.setOutlineThickness(1.f);

        Text exit_button_text("EXIT", font, 50);
        exit_button_text.setPosition(1500, 820);

        Texture menu2_background;
        menu2_background.loadFromFile("misc/menu3.png");
        Sprite menu2_background_sprite(menu2_background);
        menu2_background_sprite.setPosition(0, 0);

        vector<vector<RectangleShape>> a(n, vector<RectangleShape>(n));
        RectangleShape net(Vector2f(field_scale, field_scale));

        RectangleShape messageBox(Vector2f(400, 200));
        messageBox.setFillColor(Color(0, 0, 0, 180));
        messageBox.setPosition(1250, 430);

        Text chatTitle(L"Чат", font, 40);
        chatTitle.setFillColor(Color::White);
        chatTitle.setOutlineColor(Color::Black);
        chatTitle.setOutlineThickness(2.f);
        chatTitle.setPosition(messageBox.getPosition().x + 150, messageBox.getPosition().y - 50);

        Text messageText("", font, 24);
        messageText.setFillColor(Color::White);
        messageText.setPosition(1250, 450);

        Event event3;
        while (window.isOpen()) {
            while (window.pollEvent(event3)) {
                if (event3.type == Event::Closed) {
                    flag_window_type = 666;
                    window.close();
                }

                if (event3.type == sf::Event::KeyPressed) {
                    switch (event3.key.code)
                    {
                    case Keyboard::Up:
                        ch = 72;
                        break;
                    case Keyboard::Down:
                        ch = 80;
                        break;
                    case Keyboard::Left:
                        ch = 75;
                        break;
                    case Keyboard::Right:
                        ch = 77;
                        break;
                    default:
                        break;
                    }
                    liceyZatulinkaLogic(ch, step, moveCount);
                }

                if (button_1_mm.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                    button_1_mm.setFillColor(Color(200, 1, 1, 180));
                    if (event3.type == Event::MouseButtonPressed && event3.mouseButton.button == Mouse::Left) {
                        flag_window_type = 666;
                    }
                }
                else {
                    button_1_mm.setFillColor(Color(200, 1, 1, 200));
                }
            }

            if (flag_window_type == 666)break;
            window.clear();
            window.draw(menu2_background_sprite);
            window.draw(button_1_mm);
            window.draw(title);
            window.draw(rulesBackground); 
            window.draw(rulesTitle);   
            window.draw(rules);
            window.draw(matrixBorder);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    a[j][i] = net;
                    a[j][i].setPosition(startXfield + i * field_scale, startYfield + j * field_scale);

                    if (map1.getArray()[j][i] == 1) {
                        a[j][i].setFillColor(Color(128, 128, 128, 180));
                        small_rob.setPosition(startXfield + i * field_scale, startYfield + j * field_scale);
                        window.draw(a[j][i]);
                        window.draw(small_rob);
                    }
                    else if (map1.getArray()[j][i] == 2) {
                        a[j][i].setFillColor(Color(255, 0, 0, 180));
                        small_fox.setPosition(startXfield + i * field_scale, startYfield + j * field_scale);
                        window.draw(a[j][i]);
                        window.draw(small_fox);
                    }
                    else if (map1.getArray()[j][i] == 3) {
                        a[j][i].setFillColor(Color(139, 69, 19, 180));
                        small_bear.setPosition(startXfield + i * field_scale, startYfield + j * field_scale);
                        window.draw(a[j][i]);
                        window.draw(small_bear);
                    }
                    else {
                        if (a[j][i].getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                            a[j][i].setFillColor(Color::Yellow);
                        }
                        else {
                            a[j][i].setFillColor(Color(0, 90, 0, 200));
                        }
                        window.draw(a[j][i]);
                    }
                }
            }
            window.draw(messageBox);
            ostringstream oss;
            queue<string> tempQueue = messageQueue;
            while (!tempQueue.empty()) {
                oss << tempQueue.front() << "\n";
                tempQueue.pop();
            }
            messageText.setString(oss.str());
            window.draw(messageText);
            window.draw(chatTitle);
            window.draw(field_m3);
            window.draw(exit_button_text);
            window.draw(title);
            window.draw(matrixBorder);
            window.display();
        }
        music.stop();
    }

    void liceyZatulinkaLogic(int ch, int& step, int& moveCount) {
        if (masR.empty() && masF.empty()) {
            addMessage("BEAR TEAM WINS. Game over.");
            return;
        }
        if (masR.empty() && masB.empty()) {
            addMessage("FOX TEAM WINS. Game over.");
            return;
        }
        if (masF.empty() && masB.empty()) {
            addMessage("KROL TEAM WINS. Game over.");
            return;
        }

        addMessage("Move count: " + to_string(moveCount));

        if (!step && !masR.empty()) {
            addMessage("Krol is moving");
            for (auto& krol : masR) {
                krol->setPosAnimal(map1, ch);
            }
            step = 1;
            return;
        }
        else if (masR.empty()) {
            step = 1;
        }

        if (step == 1 && !masF.empty()) {
            addMessage("Fox is moving");
            for (auto& fox : masF) {
                fox->setPosAnimal(map1, ch);
            }
            step = 2;
        }
        else if (masF.empty()) {
            step = 2;
        }

        if (step == 2 && !masB.empty()) {
            addMessage("Bear is moving");
            for (auto& bear : masB) {
                bear->setPosAnimal(map1, ch);
                bear->eatNearbyAnimals(masR, masF, map1);
            }
            step = 3;
        }
        else if (masB.empty()) {
            step = 3;
        }

        if (step == 3) {
            for (auto foxIt = masF.begin(); foxIt != masF.end();) {
                for (auto krolIt = masR.begin(); krolIt != masR.end();) {
                    if ((*krolIt)->getX() == (*foxIt)->getX() && (*krolIt)->getY() == (*foxIt)->getY()) {
                        addMessage((*foxIt)->getName() + " killed " + (*krolIt)->getName() + " at (" + to_string((*krolIt)->getX()) + ", " + to_string((*krolIt)->getY()) + ")");
                        (*krolIt)->DeathFromFox((*krolIt)->getX(), (*krolIt)->getY(), map1);
                        krolIt = masR.erase(krolIt);
                    }
                    else {
                        ++krolIt;
                    }
                }
                ++foxIt;
            }

            step = 4;
        }

        if (step == 4) {
            addMessage("Animals aged by 1 year.");
            for (auto krolIt = masR.begin(); krolIt != masR.end();) {
                (*krolIt)->setAge((*krolIt)->getAge() + 1);
                if ((*krolIt)->Reproduce(masR, map1)) break;

                if ((*krolIt)->getAge() >= 10) {
                    addMessage((*krolIt)->getName() + " died of old age at age " + to_string((*krolIt)->getAge()) + ".");
                    (*krolIt)->deathFromTime((*krolIt)->getX(), (*krolIt)->getY(), map1);
                    krolIt = masR.erase(krolIt);
                }
                else {
                    ++krolIt;
                }
            }

            for (auto foxIt = masF.begin(); foxIt != masF.end(); ) {
                (*foxIt)->setAge((*foxIt)->getAge() + 1);
                if ((*foxIt)->getAge() >= 15) {
                    addMessage((*foxIt)->getName() + " died of old age at age " + to_string((*foxIt)->getAge()) + ".");
                    (*foxIt)->deathFromTime((*foxIt)->getX(), (*foxIt)->getY(), map1);
                    foxIt = masF.erase(foxIt);
                }
                else {
                    ++foxIt;
                }
            }

            for (auto bearIt = masB.begin(); bearIt != masB.end(); ) {
                (*bearIt)->setAge((*bearIt)->getAge() + 1);
                if ((*bearIt)->getAge() >= 20) {
                    addMessage((*bearIt)->getName() + " died of old age at age " + to_string((*bearIt)->getAge()) + ".");
                    (*bearIt)->deathFromTime((*bearIt)->getX(), (*bearIt)->getY(), map1);
                    bearIt = masB.erase(bearIt);
                }
                else {
                    ++bearIt;
                }
            }

            ++moveCount;
            step = 0;
        }
    }
};

int main()
{
    srand(time(nullptr));
    vector<Krol*> masR;
    vector<Fox*> masF;
    vector<Bear*> masB;
    Model map1(8);

    int flag_window_type = 0;
    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("misc/katya_scream.mp3");
    Sound deathSound(deathBuffer);

    RenderWindow window(VideoMode(1720, 968), "Game.exe");
    Game Game(window, map1, 8, masR, masF, masB);

    while (true) {
        if (!flag_window_type) {
            Game.drawFirstScreen(flag_window_type);
        }
        else if (flag_window_type == 1) {
            Game.drawSecondScreen(flag_window_type);
        }
        else {
            Game.drawThirdScreen(flag_window_type);
        }

        if (flag_window_type == 666)break;
    }

    for (Krol* krol : masR) {
        delete krol;
    }
    for (Fox* fox : masF) {
        delete fox;
    }
    for (Bear* bear : masB) {
        delete bear;
    }
    return 0;
}