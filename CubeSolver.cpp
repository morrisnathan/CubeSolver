#include <string>
#include <iostream>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "Cube.h"
#include "Renderer.h"

void mustInit(bool test, const std::string& description)
{
	if (test)
		return;

	std::cout << "Couldn't initialize " << description << '\n';
	exit(EXIT_FAILURE);
}

void setPerspectiveTransform(ALLEGRO_BITMAP* bmp)
{
	ALLEGRO_TRANSFORM p;
	float aspect_ratio = (float)al_get_bitmap_height(bmp) / al_get_bitmap_width(bmp);

	al_set_target_bitmap(bmp);
	al_identity_transform(&p);
	al_perspective_transform(&p, -1, aspect_ratio, 1.2, 1, -aspect_ratio, 1000);
	al_use_projection_transform(&p);
}

int main()
{
	mustInit(al_init(), "allegro");
	mustInit(al_init_image_addon(), "image addon");
	mustInit(al_init_font_addon(), "font addon");
	mustInit(al_install_keyboard(), "keyboard");

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
	mustInit(timer, "timer");

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	mustInit(queue, "queue");

	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_DEPTH_SIZE, 16, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SUPPORT_NPOT_BITMAP, 0, ALLEGRO_REQUIRE);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

	ALLEGRO_DISPLAY* disp = al_create_display(800, 600);
	mustInit(disp, "display");
	setPerspectiveTransform(al_get_backbuffer(disp));

	ALLEGRO_BITMAP* overlay = al_create_sub_bitmap(al_get_backbuffer(disp), 0, 0, 800, 600);
	mustInit(overlay, "bitmap");

	ALLEGRO_FONT* font = al_create_builtin_font();
	mustInit(font, "font");

	ALLEGRO_COLOR textColour = al_map_rgb_f(1, 1, 1);
	ALLEGRO_COLOR valueColour = al_map_rgb_f(0, 0, 1);

	mustInit(al_init_primitives_addon(), "primitives");

	ALLEGRO_BITMAP* texture = al_load_bitmap("images\\faces.png");
	mustInit(texture, "texture");

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	Renderer renderer;
	Cube cube;

	Cube::SliceRotation::Axis axis = Cube::SliceRotation::Axis::none;
	Cube::SliceRotation::Slice slice = Cube::SliceRotation::Slice::none;

	const std::map<Cube::SliceRotation::Axis, const char*> axisToString = {
		{ Cube::SliceRotation::Axis::x, "x" },
		{ Cube::SliceRotation::Axis::y, "y" },
		{ Cube::SliceRotation::Axis::z, "z" },
		{ Cube::SliceRotation::Axis::none, "none" }
	};

	const std::map<Cube::SliceRotation::Slice, const char*> sliceToString = {
		{ Cube::SliceRotation::Slice::first, "first" },
		{ Cube::SliceRotation::Slice::second, "second" },
		{ Cube::SliceRotation::Slice::third, "third" },
		{ Cube::SliceRotation::Slice::all, "all" },
		{ Cube::SliceRotation::Slice::none, "none" }
	};

	bool done = false;
	bool redraw = true;

	al_start_timer(timer);
	while (true)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);

		switch (event.type)
		{
		case ALLEGRO_EVENT_TIMER:
			if (cube.isSliceRotating())
				cube.rotateAndCheck();

			redraw = true;
			break;

		case ALLEGRO_EVENT_KEY_DOWN:
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{ 
				done = true;
			}
			else if (!cube.isSliceRotating())
			{
				switch (event.keyboard.keycode)
				{
				case ALLEGRO_KEY_X:
					axis = Cube::SliceRotation::Axis::x;
					break;
				case ALLEGRO_KEY_Y:
					axis = Cube::SliceRotation::Axis::y;
					break;
				case ALLEGRO_KEY_Z:
					axis = Cube::SliceRotation::Axis::z;
					break;
				case ALLEGRO_KEY_1:
					slice = Cube::SliceRotation::Slice::first;
					break;
				case ALLEGRO_KEY_2:
					slice = Cube::SliceRotation::Slice::second;
					break;
				case ALLEGRO_KEY_3:
					slice = Cube::SliceRotation::Slice::third;
					break;
				case ALLEGRO_KEY_A:
					slice = Cube::SliceRotation::Slice::all;
					break;
				case ALLEGRO_KEY_PAD_PLUS:
					if (axis != Cube::SliceRotation::Axis::none && slice != Cube::SliceRotation::Slice::none)
						cube.startRotating(axis, slice, true);
					break;
				case ALLEGRO_KEY_PAD_MINUS:
					if (axis != Cube::SliceRotation::Axis::none && slice != Cube::SliceRotation::Slice::none)
						cube.startRotating(axis, slice, false);
					break;
				}
			}
			break;

		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			done = true;
			break;
		}

		if (done)
			break;

		if (redraw && al_is_event_queue_empty(queue))
		{
			al_set_target_backbuffer(disp);
			al_set_render_state(ALLEGRO_DEPTH_TEST, 1);

			al_clear_to_color(al_map_rgb_f(0, 0, 0));
			al_clear_depth_buffer(1000);

			renderer.drawCube(texture, cube);

			al_set_target_bitmap(overlay);
			al_draw_text(font, textColour, 10, 10, 0, "Axis [x,y,z]:");
			al_draw_text(font, textColour, 10, 25, 0, "Slice [1,2,3,a]:");
			al_draw_text(font, textColour, 10, 40, 0, "Forward/Backward [+/-]");
			al_draw_text(font, valueColour, 145, 10, 0, axisToString.at(axis));
			al_draw_text(font, valueColour, 145, 25, 0, sliceToString.at(slice));

			al_flip_display();
			redraw = false;
		}
	}

	al_destroy_font(font);
	al_destroy_bitmap(overlay);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);

	return 0;
}
