// Copyright 2025 Greenbox
#include <SFML/Graphics.hpp>
#include <vector>
sf::RenderTexture rt({160, 144});
auto window = sf::RenderWindow(sf::VideoMode({160, 144}), "The game trademark");
char right = 0, down = 0, up = 0, left = 0, confirm = 0, cancel = 0, select = 0,start = 0;
sf::Keyboard::Key rightkey = sf::Keyboard::Key::Right,
                  downkey = sf::Keyboard::Key::Down,
                  upkey = sf::Keyboard::Key::Up,
                  leftkey = sf::Keyboard::Key::Left,
                  confirmkey = sf::Keyboard::Key::Z,
                  cancelkey = sf::Keyboard::Key::X,
                  startkey = sf::Keyboard::Key::Enter,
                  selectkey = sf::Keyboard::Key::C;
sf::View view({0.f, 0.f}, {160.f, 144.f});

float Lerp(float A, float B, float Alpha) {
  return A * (1 - Alpha) + B * Alpha;
}


struct ground{
    int x,y,x2,y2;
};
std::vector<ground> groundvector;
struct entity{
    int x=0,y=0;
    float xvelocity=0,yvelocity=0;
    int vertx=-4,verty=-8,vertx2=4,verty2=0;
};
entity player;

bool overlap(entity p,ground g)
{
   if (p.vertx+p.x >= g.x2 || g.x >= p.vertx2+p.x )
        return false;

    if (p.verty2+p.y  <= g.y || g.y2 <= p.verty+p.y)
        return false;

    return true;
}

void windowset(){
  while (std::optional event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window.close();
    }

    if (auto resized = event->getIf<sf::Event::Resized>()) {
      float x, y;
      if ((resized->size.x) > (resized->size.y)) {
        y = 144;
        x = (float(resized->size.x) / float(resized->size.y) * 144.f);
      } else {
        x = 160;
        y = (float(resized->size.y) / float(resized->size.x) * 160.f);
      }
      sf::FloatRect visibleArea({(-x + 160.f) / 2, (-y + 144.f) / 2}, {x, y});

      window.setView(sf::View(visibleArea));
    }
  }
}

void keypresscheck(sf::Keyboard::Key keycode, char* key) {
  if (sf::Keyboard::isKeyPressed(keycode)) {
    if (*key == 0)
      *key = 2;
    else if (*key == 2)
      *key = 1;
  } else {
    *key = 0;
  }
}


void update() {
  view.setCenter({Lerp(view.getCenter().x,float(player.x),0.5f),Lerp(view.getCenter().y,float(player.y),0.5f)});
  bool groundcheck=false;
  player.y++;
  for(int i=0;i<groundvector.size();i++){
    if(overlap(player,groundvector[i])){
      groundcheck=true;
      while(overlap(player,groundvector[i]))player.y--;
      break;
      }
  }
  if(!groundcheck)player.y--;

  if(groundcheck)player.yvelocity=0;
  else player.yvelocity+=0.5f;
  if(right>0)player.xvelocity+=0.5;
  else if(left>0)player.xvelocity-=0.5;
  else{
    if(player.xvelocity>0)player.xvelocity-=0.5f;
    else if(player.xvelocity<0)player.xvelocity+=0.5f;
  }
  if(player.xvelocity>2)player.xvelocity-=0.5;
  if(player.xvelocity<-2)player.xvelocity+=0.5;
  if(confirm==2&&groundcheck)player.yvelocity=-7;
  player.x+=player.xvelocity;
  player.y+=player.yvelocity;

  
}

void input(){
  keypresscheck(rightkey,&right);
  keypresscheck(leftkey,&left);
  keypresscheck(upkey,&up);
  keypresscheck(downkey,&down);
  keypresscheck(confirmkey,&confirm);
  keypresscheck(cancelkey,&cancel);
  keypresscheck(startkey,&start);
  keypresscheck(selectkey,&select);
}

void render() {
  rt.setView(view);
  window.clear();
  rt.clear();
  sf::VertexArray tri(sf::PrimitiveType::TriangleStrip, 4);
  for(int i=0;i<groundvector.size();i++){
    tri[0].position=sf::Vector2f(groundvector[i].x,groundvector[i].y);
    tri[1].position=sf::Vector2f(groundvector[i].x2,groundvector[i].y);
    tri[2].position=sf::Vector2f(groundvector[i].x,groundvector[i].y2);
    tri[3].position=sf::Vector2f(groundvector[i].x2,groundvector[i].y2);
    rt.draw(tri);
  }
  tri[0].position=sf::Vector2f(player.x+player.vertx,player.y+player.verty);
  tri[1].position=sf::Vector2f(player.x+player.vertx2,player.y+player.verty);
  tri[2].position=sf::Vector2f(player.x+player.vertx,player.y+player.verty2);
  tri[3].position=sf::Vector2f(player.x+player.vertx2,player.y+player.verty2);
  rt.draw(tri);

  rt.display();
  sf::Sprite temp(rt.getTexture());
  window.draw(temp);
  window.display();
}
void init() {
  window.setFramerateLimit(60);
  groundvector.resize(2);
  groundvector[0]=ground{-64,0,64,16};
  groundvector[1]=ground{32,-64,64,0};
}
int main() {
  init();
  while (window.isOpen()) {
    windowset();
    input();
    update();
    render();
  }
  return 0;
}
