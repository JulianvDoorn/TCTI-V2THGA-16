#pragma once

#include "PhysicsObject.hpp"
#include "Player.hpp"
class ViewFocus {
	sf::View view;
	sf::RenderWindow& window;
	PhysicsObject* focus;
	int leftBorder;
	int rightBorder;
    int topBorder;
    int bottomBorder;
public:

	/**
	 * @fn	ViewFocus::ViewFocus(sf::RenderWindow& window, PhysicsObject& physicsObject) : view(sf::Vector2f(150, 10), sf::Vector2f(1280, 720)), window(window), focus(&physicsObject)
	 *
	 * @brief	Constructs a view for window with a focus on physicsObject
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param [in,out]	window		 	The window.
	 * @param [in,out]	physicsObject	The physics object.
	 */

	ViewFocus(sf::RenderWindow& window, PhysicsObject& physicsObject) :
		view(sf::Vector2f(150, 10), sf::Vector2f(1280, 720)),
		window(window),
		focus(&physicsObject)
	{ }

	/**
	 * @fn	ViewFocus::ViewFocus(sf::RenderWindow& window) : view(sf::Vector2f(150, 10), sf::Vector2f(1280, 720)), window(window), focus(nullptr)
	 *
	 * @brief	Constructs a view for window without a focus.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param [in,out]	window	The window.
	 */

	ViewFocus(sf::RenderWindow& window) :
		view(sf::Vector2f(150, 10), sf::Vector2f(1280, 720)),
		window(window),
		focus(nullptr)
	{ }

	/**
	 * @fn	void ViewFocus::setFocus(PhysicsObject& focus)
	 *
	 * @brief	Sets the focus of ViewFocus on focus
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param [in,out]	focus	The focus to apply on ViewFocus.
	 */

	void setFocus(PhysicsObject& focus) {
		this->focus = &focus;
        view.setCenter(focus.getPosition());
	}

	/**
	 * @fn	void ViewFocus::unsetFocus()
	 *
	 * @brief	Unsets focus from any object
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 */
	void setLeftBorder(int border){
		leftBorder = border;
	}
	void  setRightBorder(int border){
		rightBorder= border;
	}
    void setTopBorder(int border){
        topBorder = border;
    }
    void setBottomBorder(int border){
        bottomBorder = border;
    }
	int getLeftBorder(){
		return leftBorder;
	}
	int getRightBorder(){
		return rightBorder;
	}
    int getTopBorder(){
        return topBorder;
    }
    int getBottomBorder(){
        return  bottomBorder;
    }
	void unsetFocus() {
		this->focus = nullptr;
	}

	/**
	 * @fn	void ViewFocus::update()
	 *
	 * @brief	Updates this object
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 */

	void update() {
		// Check if focus object is outside of border and how much it is outside the border. Move view the delta of border and focus object position.
        int leftDiff = (focus->getPosition().x - (view.getCenter().x  -(view.getSize().x/2)+leftBorder));
        int rightDiff = (focus->getPosition().x - (view.getCenter().x + (view.getSize().x/2) - rightBorder));

        int topDiff = (focus->getPosition().y - (view.getCenter().y + (view.getSize().y/2) -topBorder));
        int bottomDiff = (focus->getPosition().y -( view.getCenter().y - (view.getSize().y/2) +bottomBorder));

		if (leftDiff < 0 || rightDiff > 0 || topDiff < 0 || bottomDiff > 0 ){
			if (focus != nullptr) {
                if ( leftDiff< 0){
                    view.move(leftDiff,0.0);
                }
                else if (rightDiff >0 ){
                    view.move(rightDiff,0.0);
                }
                else if (topDiff >0 ){
                    view.move(0.0,topDiff);
                }
                else if (bottomDiff < 0){
                    view.move(0.0,bottomDiff);
                }
			} else {
				view.setCenter(window.getDefaultView().getCenter());
			}
		}

		window.setView(view);
	}
};