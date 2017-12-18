#include <gtkmm.h>
#include "SFMLWidget.hpp"

class MovingCircle {
public:
	// A simple circle shape that will be animated and drawn
	sf::CircleShape circle;
	
	// a reference to our SFMLWidget
	SFMLWidget &widget;
	
	// The radius of our circle
	float radius;
	
	explicit MovingCircle(SFMLWidget &widget) : widget(widget) {
		// Set the radius to an unmiportand value
		radius = 32.f;
		circle.setRadius(radius);
		
		// move the circoe to it's first position
		moveToStartPoint();
		
		// Let the animate method be called every 50ms
		// Note: MovingCircle::animate() doesn't return any value, but signal_timeout() expects
		//       a boolean value.
		//       Using sigc::bind_return(true, ...) we get a signal returning always true.
		Glib::signal_timeout().connect(sigc::bind_return(sigc::mem_fun(this, &MovingCircle::animate), true), 50);
		
		// Makes our draw Method beeing drawn everytime the widget itself gets drawn.
		// Note: MovingCircle::draw() doesn't accept any parameter, but signal_draw() gives one.
		//       Using sigc::hide(...) we get a signal expecting one.
		widget.signal_draw().connect(sigc::bind_return(sigc::hide(sigc::mem_fun(this, &MovingCircle::draw)), true));
		// NOTE: in earlier gtkmm-versions (<3.0) instead of signal_draw, connext to signal_event_expose:
		// widget.signal_event_expose().connect(sigc::bind_return(
		//                                          sigc::hide(
		//                                              sigc::mem_fun(this, &MovingCircle::draw)),
		//                                          true));
		
		// Everytime the widget gets resized, we need to adjust the view.
		widget.signal_size_allocate().connect(sigc::hide(sigc::mem_fun(this, &MovingCircle::resize_view)));
	}
	
	void animate() {
		// Simply move the circle...
		sf::Vector2f position = circle.getPosition();
		position.x += 8.f;
		position.y += 8.f;
		
		// until it "leaves" the Widget
		if (position.x > widget.renderWindow.getSize().x + radius || position.y > widget.renderWindow.getSize().y + radius) {
			moveToStartPoint();
		} else {
			circle.setPosition(position);
		}
		
		// Tell gtkmm that the SFML Widget wants to be redrawn
		widget.invalidate();
	}
	
	void draw() {
		widget.renderWindow.clear();
		
		widget.renderWindow.draw(circle);
		
		// Calls SFMLWidget::display, whitch checks wether the widget is realized
		// and if so, sf::RenderWindow::display gets called.
		widget.display();
	}
	
	void resize_view() {
		// Let the View fit the pixels of the window.
		sf::Vector2f lower_right(widget.renderWindow.getSize().x, widget.renderWindow.getSize().y);
		
		sf::View view(lower_right * 0.5f, lower_right);
		widget.renderWindow.setView(view);
	}
	
	void moveToStartPoint() {
		circle.setPosition(-radius, -radius);
	}
};

int main(int argc, char *argv[]) {
	Gtk::Main kit(argc, argv); //Initialize Gtk
	
	Gtk::Window window; //The GTK window will be our top level Window
	
	//Our RenderWindow will never be below  640x480 (unless we explicitly change it)
	//but it may be more then that
	SFMLWidget ourRenderWindow(sf::VideoMode(640, 480));
	
	MovingCircle moving_circle(ourRenderWindow);
	
	// Doesn't draw the renderWindow but makes it so it will draw when we add it to the window
	ourRenderWindow.show();
	
	//VBox is a vertical box, we're going to pack our render window and a button in here
	Gtk::VBox ourVBox;
	
	Gtk::Button ourButton("Restart"); //Just a clickable button, it won't be doing anything
	ourButton.show();
	ourButton.signal_clicked().connect(sigc::mem_fun(&moving_circle, &MovingCircle::moveToStartPoint));
	
	Gtk::Button ourButton0("Dummy"); //Just a clickable button, it won't be doing anything
	ourButton0.show();
	
	ourVBox.pack_start(ourRenderWindow); //Add ourRenderWindow to the top of the VBox
	ourVBox.pack_start(ourButton0, Gtk::PACK_SHRINK);
	
	//PACK_SHRINK makes the VBox only allocate enough space to show the button and nothing more
	ourVBox.pack_start(ourButton, Gtk::PACK_SHRINK);
	ourVBox.show();
	
	window.add(ourVBox); //Adds ourVBox to the window so it (and it's children) can be drawn
	
	Gtk::Main::run(window); //Draw the window
	return 0;
}