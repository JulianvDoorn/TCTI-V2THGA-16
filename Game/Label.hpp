#pragma  once

#include <SFML/Graphics.hpp>

/**
 * @class	Label
 *
 * @brief	A displayable label.
 *
 * @author	Wiebe
 * @date	25-1-2018
 */

class Label : public sf::Drawable {
private:

    /** @brief	The font used */
    sf::Font &font;

    /** @brief	The text that the label holds */
    sf::Text displayText;
public:
    /**
     *  @brief Get the stored default font.
     * @return Reference to the default font.
     */
    static sf::Font& getDefaultFont() {
        static sf::Font font;
        return font;
    }
    /**
     *  @brief Store the default font.
     * @param font  The new default font.
     */
    static void setDefaultFont(sf::Font &font) {
		getDefaultFont() = font;
    }

    /**
     *  @brief Empty constructor for a label with the default font.
     */
    Label() : font(getDefaultFont()){
        displayText.setFont(font);
    }
    /**
     * @brief Label constructor with a custom font.
     * @param font A font.
     */
	Label(sf::Font& font) : font(font) {
        displayText.setFont(font);
    }

    /**
     * @brief Lable constructor with all options, some a optional.
     * @param text The text that will be displayed.
     * @param font The font used to display the text.
     * @param position The position the label will be placed at.
     * @param size The charactersize the text will be displayed in.
     * @param color Optional. The color the text will be displayed in.
     * @param style Optional. The style the text will be displayed in.
     */
    Label(std::string &text, sf::Font &font, sf::Vector2f position, int size, sf::Color color = sf::Color::White, sf::Text::Style style = sf::Text::Bold) : font(font) {
        displayText.setPosition(position);
        displayText.setFont(font);
        displayText.setFillColor(color);
        displayText.setCharacterSize(size);
        displayText.setStyle(style);
        displayText.setPosition(position);
        displayText.setString(text);
    }

    /**
     * @brief Set the font the text will be displayed in.
     * @param font A custom font.
     */
	void setFont(sf::Font& font) {
		displayText.setFont(font);
	}

    /**
     * @brief The text that will be displayed on the label.
     * @param text A string that will be displayed.
     */
    void setText(const std::string& text){
        displayText.setString(text);
    }

    /**
     * @brief Get the text that is displayed.
     * @return The displayed text.
     */
	std::string getText() const {
		return displayText.getString();
	}

    /**
     * @brief Set the color of the text.
     * @param color The color of the text.
     */
    void setColor(sf::Color color){
        displayText.setFillColor(color);
    }

    /**
     * @brief Set the position the text will be displayed at.
     * @param position Vector of the positon..
     */
    void setPosition(sf::Vector2f position){
        displayText.setPosition(position);
    }

    /**
     * @brief Set the size of the characters in the text.
     * @param size The size of a character.
     */
    void setCharSize(int size){
        displayText.setCharacterSize(size);
    }

    /**
     * @brief Set the origin of the text using a vector.
     * @param position A vector with the  new position of the origin.
     */
    void setOrigin(sf::Vector2f position){
        displayText.setOrigin(position);
    }

    /**
     * @brief Set the origin of the the text using a x and y coordinate.
     * @param x The x coordinate of the new origin.
     * @param y The y coordinate of the new origin.
     */
    void setOrigin(float x, float y){
        displayText.setOrigin({x,y});
    }

    /**
     * @brief Draw the text on a window.
     * @param window Reference to window the text will be displayed on.
     */
    void draw(sf::RenderTarget &window, sf::RenderStates states) const {
        window.draw(displayText);
    }

    /**
     * @brief Get the outer bounds of all the text.
     * @return Body of the outer bounds of the text.
     */
    sf::FloatRect getBounds(){
        return displayText.getLocalBounds();
    }

    /**
     * @brief Set the sytle of the text, for example bold or italic..
     * @param style The new style for the text.
     */
    void setStyle(sf::Text::Style style){
        displayText.setStyle(style);
    }
    void  setOutlineColor(const sf::Color & color){
        displayText.setOutlineColor(color);
    }
    void setOutlineThicknes(float size){
        displayText.setOutlineThickness(size);
    }
};

