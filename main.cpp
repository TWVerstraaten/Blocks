#include "Application_Edit.h"
#include "Application_Run.h"
#include "aux/Aux.h"

int main(int argc, char* argv[]) {

    view::View v{};

    Application_Edit app(&v);
    app.loop();

    Application_Run app2(app.model(), &v);
    app2.loop();

    //    Point p{0, 2};
    //
    //    SDL_Rect r{-1, -1, 2, 2};
    //    std::cout << aux::pointInAlignedRectangle(r, p);

    //    Point point{1, 0};
    //    Point res = aux::rotateClockWiseAboutPivot(point, {2, 1}, 90);
    //    std::cout << res.x() << " " << res.y() << '\n';
}
