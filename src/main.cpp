#include "app.h"
#include "defs.h"

int main(int argc, char** argv) {
	// get rid of warnings when compile (unused variable)
	(void)argc;
	(void)argv;
	// run app
	App* app = new App();
	app->Run();
	delete app;
	return 0;
}
