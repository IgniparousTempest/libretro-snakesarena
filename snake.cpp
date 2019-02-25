#include "snake.hpp"
#include "engine/image.hpp"

bool IsRight(Direction main, Direction other) {
    switch (main) {
        case UP:
            return other == RIGHT;
        case DOWN:
            return other == LEFT;
        case LEFT:
            return other == UP;
        case RIGHT:
            return other == DOWN;
    }
    return false;
}

Snake::Snake(int square_size, int field_width, int field_height, std::pair<Pos, Direction> spawn, SnakeAssets *assets) :
head(assets->head), body_straight(assets->body_straight), body_turn(assets->body_turn), tail(assets->tail_straight),
tail_turn_left(assets->tail_turn_left), tail_turn_right(assets->tail_turn_right), field_width(field_width),
field_height(field_height) {
    redraw_body = true;
    moves_queue.push_back(spawn.second);
    body.push_back(spawn.first);
    body.push_back(Auxiliary::ApplyDirection(body.back(), Auxiliary::OppositionDirection(spawn.second)));
    body.push_back(Auxiliary::ApplyDirection(body.back(), Auxiliary::OppositionDirection(spawn.second)));
    body_orientations.push_front(moves_queue.front());
    body_orientations.push_front(moves_queue.front());
    body_orientations.push_front(moves_queue.front());
    body_desired_size = body.size();

    screen_rect = {0, 0, square_size, square_size};
    delay = 1.0 / speed;
}

void Snake::Update(double delta_time) {
    delay -= delta_time;
    if (delay <= 0)
        can_make_move = true;
}

void Snake::Render(Renderer *renderer, Field *field) {
    Direction previous_dir = body_orientations[0];
    Texture *texture;
    int angle = 0;
    for (int i = 0; i < body.size(); ++i) {
        if (i == 0)
            texture = head;
        else if (i == body.size() - 1) {
            if (body_orientations[i] == body_orientations[i - 1])
                texture = tail;
            else if (IsRight(body_orientations[i], body_orientations[i - 1]))
                texture = tail_turn_right;
            else
                texture = tail_turn_left;
        }
        else if (body_orientations[i] == previous_dir)
            texture = body_straight;
        else {
            previous_dir = body_orientations[i];
            texture = body_turn;
        }
        switch (body_orientations[i]) {
            case UP:
                angle = 270;
                break;
            case DOWN:
                angle = 90;
                break;
            case LEFT:
                angle = 180;
                break;
            case RIGHT:
                angle = 0;
                break;
        }
        if ((texture == body_turn) && IsRight(body_orientations[i], body_orientations[i - 1]))
            angle = ((angle - 90) % 360 + 360) % 360;
        screen_rect.x = field->ScreenX(body[i].x);
        screen_rect.y = field->ScreenY(body[i].y);
        renderer->Render(texture, &screen_rect, angle);
    }
}

void Snake::MoveUp() {
    if (moves_queue.size() < MOVES_QUEUE_MAX_SIZE)
        moves_queue.push_back(UP);
}

void Snake::MoveDown() {
    if (moves_queue.size() < MOVES_QUEUE_MAX_SIZE)
        moves_queue.push_back(DOWN);
}

void Snake::MoveLeft() {
    if (moves_queue.size() < MOVES_QUEUE_MAX_SIZE)
        moves_queue.push_back(LEFT);
}

void Snake::MoveRight() {
    if (moves_queue.size() < MOVES_QUEUE_MAX_SIZE)
        moves_queue.push_back(RIGHT);
}

void Snake::GrowBody(size_t delta) {
    body_desired_size += delta;
}

Direction Snake::PeekNextMove() {
    Direction next_move;
    while (true) {
        if (!moves_queue.empty()) {
            next_move = moves_queue.front();
            // Skip move if not valid.
            if ((next_move == UP && body_orientations.front() == DOWN) ||
            (next_move == DOWN && body_orientations.front() == UP) ||
            (next_move == LEFT && body_orientations.front() == RIGHT) ||
            (next_move == RIGHT && body_orientations.front() == LEFT)) {
                moves_queue.pop_front();
                continue;
            }
            return next_move;
        } else {
            // IF queue is empty use current direction.
            return body_orientations.front();
        }
    }
}

Direction Snake::PopNextMove() {
    Direction next_move = PeekNextMove();
    if (!moves_queue.empty())
        moves_queue.pop_front();

    return next_move;
}

bool Snake::CanMakeMove(Pos *position) {
    *position = ApplyDirection(body.front(), PeekNextMove());
    return can_make_move;
}

void Snake::MakeMove() {
    if (!can_make_move)
        return;
    Pos head = body.front();
    for (auto segment : body)
        need_to_be_updated.push_back(segment);
    redraw_body = true;
    if (body.size() >= body_desired_size) {
        body.pop_back();
        body_orientations.pop_back();
    }
    delay = 1.0 / speed;
    Direction next_move = PopNextMove();
    // Make move
    head = ApplyDirection(head, next_move);
    body_orientations.push_front(next_move);
    body.push_front(head);

    can_make_move = false;
}

Pos Snake::ApplyDirection(Pos pos, Direction move) {
    Pos next = pos;
    switch (move) {
        case UP:
            if (--next.y < 0)
                next.y = field_height - 1;
            break;
        case DOWN:
            if (++next.y >= field_height)
                next.y = 0;
            break;
        case LEFT:
            if (--next.x < 0)
                next.x = field_width - 1;
            break;
        case RIGHT:
            if (++next.x >= field_width)
                next.x = 0;
            break;
    }
    return next;
}
