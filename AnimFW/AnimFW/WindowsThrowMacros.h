#pragma once

#define CUWND_EXCEPT(hr) Window::HRException(__LINE__, __FILE__, hr)
#define CUWND_LAST_EXCEPT() Window::HRException(__LINE__, __FILE__, GetLastError())
#define CUWND_NOGFX_EXCEPT() Window::NoGFXException( __LINE__,__FILE__ )
