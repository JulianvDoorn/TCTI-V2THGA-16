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
	bool dynamicCamera = true;
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
	 * @fn	void ViewFocus::setDynamicCameraEnabled(bool dynamicCamera)
	 *
	 * @brief	Sets dynamic camera enabled
	 *
	 * @author	Julian
	 * @date	2018-01-25
	 *
	 * @param	dynamicCamera	Boolean to set
	 */

	void setDynamicCameraEnabled(bool dynamicCamera) {
		this->dynamicCamera = dynamicCamera;
	}

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
    void unsetFocus() {
        this->focus = nullptr;
    }

    /**
     * @brief Set the left border from where the camera will start following the focus object.
     * @param [in] border Distance from left side of the screen in pixels.
     */
	void setLeftBorder(int border){
		leftBorder = border;
	}
    /**
     *  @brief Set the right border from where the camera will start following the focus object.
     * @param [in]  border Distance from the right side of the screen in pixels..
     */
	void  setRightBorder(int border){
		rightBorder= border;
	}

    /**
     * @brief Set the top border from where the camera will start following the focus object.
     * @param border [in] Distance from the top of the screen in pixels.
     */
    void setTopBorder(int border){
        topBorder = border;
    }

    /**
     * @brief Set the bottom border from where the camera will start following the focus object.
     * @param border [in]  Distance from the bottom of the screen in pixels.
     */
    void setBottomBorder(int border){
        bottomBorder = border;
    }

    /**
     * @brief Get the left border from where the camera will start following the focus object.
     * @return The left border in pixels.
     */
	int getLeftBorder(){
		return leftBorder;
	}
    /**
     * @brief Get the right border from where the camera will start following the focus object.
     * @return The right border in pixels.
     */
	int getRightBorder(){
		return rightBorder;
	}
    /**
     * @brief Get the top border from where the camera will start following the focus object.
     * @return The top border in pixels.
     */
    int getTopBorder(){
        return topBorder;
    }
    /**
     * @brief Get the bottom border from where the camera will start following the focus object.
     * @return The bottom border in pixels.
     */
    int getBottomBorder(){
        return  bottomBorder;
    }


    /**
     *  @brief Update the view by checking if the focus object is ouside of the set borders, if focus object is outside of the border the view will be moved by the difference between the view and the focus object.
     */
	void update() {
        // Check if focus is defined,  it can be undefined  if state is changing for example.
		if (focus != nullptr) {
			if (dynamicCamera) {
				// Check if focus object is outside of border and how much it is outside the border. Move view the delta of border and focus object position.
				float leftDiff = (focus->getPosition().x - (view.getCenter().x - (view.getSize().x / 2) + leftBorder));
				float rightDiff = (focus->getPosition().x - (view.getCenter().x + (view.getSize().x / 2) - rightBorder));
				float topDiff = (focus->getPosition().y - (view.getCenter().y + (view.getSize().y / 2) - topBorder));
				float bottomDiff = (focus->getPosition().y - (view.getCenter().y - (view.getSize().y / 2) + bottomBorder));

				// Check if ouside of border
				if (leftDiff < 0 || rightDiff > 0 || topDiff < 0 || bottomDiff > 0) {
					if (leftDiff < 0) {
						view.move(leftDiff, 0.0);
					}
					else if (rightDiff > 0) {
						view.move(rightDiff, 0.0);
					}
					else if (topDiff > 0) {
						view.move(0.0, topDiff);
					}
					else if (bottomDiff < 0) {
						view.move(0.0, bottomDiff);
					}
				}
			}
			else {
				view.setCenter(focus->getPosition());
			}
		} else {
			// Set default view if no focus object is defined.
			view.setCenter(window.getDefaultView().getCenter());
		}

		window.setView(view);
	}
};