//тестовый код урока №1
#include <SFML/Graphics.hpp>

int main()
{
    // Создаём окно с той же битовой глубиной пикселей, что и рабочий стол
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(200, 200, desktop.bitsPerPixel), "Lesson 1");
    sf::CircleShape shape(100.f); //Рисуем круг
    shape.setFillColor(sf::Color::Red); //Заливаем круг красным цветом

    // Основной (бесконечный) цикл
    while (window.isOpen())
    {
        sf::Event event; //Переменная для события

        while (window.pollEvent(event)) //Опрос событий
        {
            if (event.type == sf::Event::Closed)
            window.close();//Закрываем окно, если событие “Closed”
        }

        window.clear(); //Очищаем экран
        window.draw(shape); //Рисуем круг
        window.display(); //Отображаем круг на экран
    }
    return 0;
}
