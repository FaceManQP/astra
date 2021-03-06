#include <SFML/Graphics.hpp>
#include <Astra/Astra.h>
#include <AstraUL/AstraUL.h>
#include "../../common/LitDepthVisualizer.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <key_handler.h>
#include <sstream>

class DepthFrameListener : public astra::FrameReadyListener
{
public:
    DepthFrameListener(const astra::CoordinateMapper& coordinate_mapper) :
        m_coordinate_mapper(coordinate_mapper)
    {
        m_lastTimepoint = clock_type::now();
        m_font.loadFromFile("Inconsolata.otf");
    }

    void init_texture(int width, int height)
    {
        if (m_displayBuffer == nullptr || width != m_displayWidth || height != m_displayHeight)
        {
            m_displayWidth = width;
            m_displayHeight = height;

            // texture is RGBA
            int byteLength = m_displayWidth * m_displayHeight * 4;

            m_displayBuffer = BufferPtr(new uint8_t[byteLength]);
            memset(m_displayBuffer.get(), 0, byteLength);

            m_texture.create(m_displayWidth, m_displayHeight);
            m_sprite.setTexture(m_texture);
            m_sprite.setPosition(0, 0);
        }
    }

    void check_fps()
    {
        const double frameWeight = 0.2;

        auto newTimepoint = clock_type::now();
        auto frameDuration = std::chrono::duration_cast<duration_type>(newTimepoint - m_lastTimepoint);

        m_frameDuration = frameDuration * frameWeight + m_frameDuration * (1 - frameWeight);
        m_lastTimepoint = newTimepoint;

        double fps = 1.0 / m_frameDuration.count();

        auto precision = std::cout.precision();
        std::cout << std::fixed
                  << std::setprecision(1)
                  << fps << " fps ("
                  << std::setprecision(2)
                  << frameDuration.count() * 1000 << " ms)"
                  << std::setprecision(precision)
                  << std::endl;
    }

    virtual void on_frame_ready(astra::StreamReader& reader,
                                astra::Frame& frame) override
    {
        astra::PointFrame pointFrame = frame.get<astra::PointFrame>();

        int width = pointFrame.resolutionX();
        int height = pointFrame.resolutionY();

        init_texture(width, height);

        check_fps();

        if (m_isPaused)
        {
            return;
        }

        copy_depth_data(frame);

        m_visualizer.update(pointFrame);

        astra_rgb_pixel_t* vizBuffer = m_visualizer.get_output();
        for (int i = 0; i < width * height; i++)
        {
            int rgbaOffset = i * 4;
            m_displayBuffer[rgbaOffset] = vizBuffer[i].r;
            m_displayBuffer[rgbaOffset + 1] = vizBuffer[i].b;
            m_displayBuffer[rgbaOffset + 2] = vizBuffer[i].g;
            m_displayBuffer[rgbaOffset + 3] = 255;
        }
        m_texture.update(m_displayBuffer.get());
    }

    void copy_depth_data(astra::Frame& frame)
    {
        astra::DepthFrame depthFrame = frame.get<astra::DepthFrame>();

        if (depthFrame.is_valid())
        {
            int width = depthFrame.resolutionX();
            int height = depthFrame.resolutionY();
            if (m_depthData == nullptr || width != m_depthWidth || height != m_depthHeight)
            {
                m_depthWidth = width;
                m_depthHeight = height;

                // texture is RGBA
                int byteLength = m_depthWidth * m_depthHeight * sizeof(uint16_t);

                m_depthData = DepthPtr(new int16_t[byteLength]);
            }
            depthFrame.copy_to(m_depthData.get());
        }
    }

    void update_mouse_position(sf::RenderWindow& window)
    {
        sf::Vector2i position = sf::Mouse::getPosition(window);
        auto windowSize = window.getSize();

        m_mouseNormX = position.x / static_cast<float>(windowSize.x);
        m_mouseNormY = position.y / static_cast<float>(windowSize.y);
    }

    void drawText(sf::RenderWindow& window, sf::Text& text, sf::Color color, int x, int y)
    {
        text.setColor(sf::Color::Black);
        text.setPosition(x + 5, y + 5);
        window.draw(text);

        text.setColor(color);
        text.setPosition(x, y);
        window.draw(text);
    }

    void drawMouseOverlay(sf::RenderWindow& window, float depthWScale, float depthHScale)
    {
        if (!m_isMouseOverlayEnabled || m_depthData == nullptr)
        {
            return;
        }
        int mouseX = m_depthWidth * m_mouseNormX;
        int mouseY = m_depthHeight * m_mouseNormY;

        if (mouseX >= m_depthWidth || mouseY >= m_depthHeight ||
            mouseX < 0 || mouseY < 0)
        {
            return;
        }

        size_t index = (m_depthWidth * mouseY + mouseX);

        int z = m_depthData[index];

        float worldX, worldY, worldZ;
        m_coordinate_mapper.convert_depth_to_world(static_cast<float>(mouseX),
                                                   static_cast<float>(mouseY),
                                                   static_cast<float>(z),
                                                   &worldX,
                                                   &worldY,
                                                   &worldZ);

        std::stringstream str;
        str << std::fixed << std::setprecision(0);
        str << "(" << mouseX << ", " << mouseY << ") X:" << worldX << " Y:" << worldY << " Z:" << worldZ;
        sf::Text text(str.str(), m_font);

        int characterSize = 40;
        text.setCharacterSize(characterSize);
        text.setStyle(sf::Text::Bold);

        float display_x = 10;
        float display_y = window.getView().getSize().y - 10 - characterSize;
        drawText(window, text, sf::Color::White, display_x, display_y);
    }

    void drawTo(sf::RenderWindow& window)
    {
        if (m_displayBuffer != nullptr)
        {
            float depthWScale = window.getView().getSize().x / m_displayWidth;
            float depthHScale = window.getView().getSize().y / m_displayHeight;

            m_sprite.setScale(depthWScale, depthHScale);

            window.draw(m_sprite);

            drawMouseOverlay(window, depthWScale, depthHScale);
        }
    }

    void toggle_isPaused()
    {
        m_isPaused = !m_isPaused;
    }

    bool get_isPaused() const
    {
        return m_isPaused;
    }
    
    void toggle_isMouseOverlayEnabled()
    {
        m_isMouseOverlayEnabled = !m_isMouseOverlayEnabled;
    }

    bool get_isMouseOverlayEnabled() const
    {
        return m_isMouseOverlayEnabled;
    }

private:
    samples::common::LitDepthVisualizer m_visualizer;

    using duration_type = std::chrono::duration<double>;
    duration_type m_frameDuration{0.0};

    using clock_type = std::chrono::system_clock;
    std::chrono::time_point<clock_type> m_lastTimepoint;
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    sf::Font m_font;

    const astra::CoordinateMapper& m_coordinate_mapper;

    using BufferPtr = std::unique_ptr< uint8_t[] >;
    BufferPtr m_displayBuffer{ nullptr };
    int m_displayWidth{ 0 };
    int m_displayHeight{ 0 };

    int m_depthWidth{ 0 };
    int m_depthHeight{ 0 };
    using DepthPtr = std::unique_ptr < int16_t[] > ;
    DepthPtr m_depthData{ nullptr };

    float m_mouseNormX{ 0 };
    float m_mouseNormY{ 0 };
    bool m_isPaused{ false };
    bool m_isMouseOverlayEnabled{ true };
};


int main(int argc, char** argv)
{
    astra::Astra::initialize();

    set_key_handler();

    sf::RenderWindow window(sf::VideoMode(1280, 960), "Depth Viewer");

    #ifdef _WIN32
        auto fullscreenStyle = sf::Style::None;
    #else
        auto fullscreenStyle = sf::Style::Fullscreen;
    #endif

    sf::VideoMode fullscreen_mode = sf::VideoMode::getFullscreenModes()[0];
    sf::VideoMode windowed_mode(1280, 960);
    bool is_fullscreen = false;

    astra::StreamSet streamset;
    astra::StreamReader reader = streamset.create_reader();

    reader.stream<astra::PointStream>().start();
    auto depthStream = reader.stream<astra::DepthStream>();
    depthStream.start();

    auto coordinate_mapper = depthStream.coordinateMapper();

    DepthFrameListener listener(coordinate_mapper);

    reader.addListener(listener);

    while (window.isOpen())
    {
        astra_temp_update();

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
            {
                if (event.key.code == sf::Keyboard::C && event.key.control)
                {
                    window.close();
                }
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        window.close();
                        break;
                    case sf::Keyboard::F:
                        if (is_fullscreen)
                        {
                            is_fullscreen = false;
                            window.create(windowed_mode, "Depth Viewer", sf::Style::Default);
                        }
                        else
                        {
                            is_fullscreen = true;
                            window.create(fullscreen_mode, "Depth Viewer", fullscreenStyle);
                        }
                        break;
                    case sf::Keyboard::R:
                        depthStream.enable_registration(!depthStream.registration_enabled());
                        break;
                    case sf::Keyboard::M:
                        depthStream.enable_mirroring(!depthStream.mirroring_enabled());
                        break;
                    case sf::Keyboard::P:
                        listener.toggle_isPaused();
                        break;
                    case sf::Keyboard::Space:
                        listener.toggle_isMouseOverlayEnabled();
                        break;
                    default:
                        break;
                }
                break;
            }
            case sf::Event::MouseMoved:
            {
                listener.update_mouse_position(window);
                break;
            }
            default:
                break;
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        listener.drawTo(window);
        window.display();

        if (!shouldContinue)
        {
            window.close();
        }
    }

    astra::Astra::terminate();
    return 0;
}

