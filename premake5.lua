project "GLFW"
    kind "StaticLib"
    language "C"
    staticruntime "off"
    warnings "off"

    targetdir ("../../../Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("../../../Intermediate/" .. outputdir .. "/%{prj.name}")

    includedirs { VULKAN_SDK .. "/Include" }

    files {
        "include/GLFW/glfw3.h",
        "include/GLFW/glfw3native.h",
        "src/context.c",
        "src/init.c",
        "src/input.c",
        "src/monitor.c",
        "src/null_init.c",
        "src/null_joystick.c",
        "src/null_monitor.c",
        "src/null_window.c",
        "src/platform.c",
        "src/vulkan.c",
        "src/window.c"
    }

    filter "system:linux"
        pic "On"
        systemversion "latest"

        local wl_proto_dir = "/usr/share/wayland-protocols"
        local wl_base_dir  = "/usr/share/wayland"

        prebuildcommands {
            -- Core Wayland
            "wayland-scanner client-header < " .. wl_base_dir .. "/wayland.xml > src/wayland-client-protocol.h",
            "wayland-scanner private-code < " .. wl_base_dir .. "/wayland.xml > src/wayland-client-protocol-code.h",

            -- XDG shell
            "wayland-scanner client-header < " .. wl_proto_dir .. "/stable/xdg-shell/xdg-shell.xml > src/xdg-shell-client-protocol.h",
            "wayland-scanner private-code < " .. wl_proto_dir .. "/stable/xdg-shell/xdg-shell.xml > src/xdg-shell-client-protocol-code.h",

            -- XDG decoration
            "wayland-scanner client-header < " .. wl_proto_dir .. "/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml > src/xdg-decoration-unstable-v1-client-protocol.h",
            "wayland-scanner private-code < " .. wl_proto_dir .. "/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml > src/xdg-decoration-unstable-v1-client-protocol-code.h",

            -- Viewporter
            "wayland-scanner client-header < " .. wl_proto_dir .. "/stable/viewporter/viewporter.xml > src/viewporter-client-protocol.h",
            "wayland-scanner private-code < " .. wl_proto_dir .. "/stable/viewporter/viewporter.xml > src/viewporter-client-protocol-code.h",

            -- Relative pointer
            "wayland-scanner client-header < " .. wl_proto_dir .. "/unstable/relative-pointer/relative-pointer-unstable-v1.xml > src/relative-pointer-unstable-v1-client-protocol.h",
            "wayland-scanner private-code < " .. wl_proto_dir .. "/unstable/relative-pointer/relative-pointer-unstable-v1.xml > src/relative-pointer-unstable-v1-client-protocol-code.h",

            -- Pointer gestures
            "wayland-scanner client-header < " .. wl_proto_dir .. "/unstable/pointer-gestures/pointer-gestures-unstable-v1.xml > src/pointer-gestures-unstable-v1-client-protocol.h",
            "wayland-scanner private-code < " .. wl_proto_dir .. "/unstable/pointer-gestures/pointer-gestures-unstable-v1.xml > src/pointer-gestures-unstable-v1-client-protocol-code.h",

            -- Fractional scale
            "wayland-scanner client-header < " .. wl_proto_dir .. "/staging/fractional-scale/fractional-scale-v1.xml > src/fractional-scale-v1-client-protocol.h",
            "wayland-scanner private-code < " .. wl_proto_dir .. "/staging/fractional-scale/fractional-scale-v1.xml > src/fractional-scale-v1-client-protocol-code.h",

            -- Idle inhibit
            "wayland-scanner client-header < " .. wl_proto_dir .. "/unstable/idle-inhibit/idle-inhibit-unstable-v1.xml > src/idle-inhibit-unstable-v1-client-protocol.h",
            "wayland-scanner private-code < " .. wl_proto_dir .. "/unstable/idle-inhibit/idle-inhibit-unstable-v1.xml > src/idle-inhibit-unstable-v1-client-protocol-code.h",

            -- Pointer constraints
            "wayland-scanner client-header < " .. wl_proto_dir .. "/unstable/pointer-constraints/pointer-constraints-unstable-v1.xml > src/pointer-constraints-unstable-v1-client-protocol.h",
            "wayland-scanner private-code < " .. wl_proto_dir .. "/unstable/pointer-constraints/pointer-constraints-unstable-v1.xml > src/pointer-constraints-unstable-v1-client-protocol-code.h",

            -- Tablet
            "wayland-scanner client-header < " .. wl_proto_dir .. "/unstable/tablet/tablet-unstable-v2.xml > src/tablet-unstable-v2-client-protocol.h",
            "wayland-scanner private-code < " .. wl_proto_dir .. "/unstable/tablet/tablet-unstable-v2.xml > src/tablet-unstable-v2-client-protocol-code.h",

            -- XDG activation
            "wayland-scanner client-header < " .. wl_proto_dir .. "/staging/xdg-activation/xdg-activation-v1.xml > src/xdg-activation-v1-client-protocol.h",
            "wayland-scanner private-code < " .. wl_proto_dir .. "/staging/xdg-activation/xdg-activation-v1.xml > src/xdg-activation-v1-client-protocol-code.h",
        }

        files {
            -- Wayland sources
            "src/wl_init.c",
            "src/wl_monitor.c",
            "src/wl_window.c",

            -- X11/GLX sources
            "src/x11_init.c",
            "src/x11_monitor.c",
            "src/x11_window.c",
            "src/glx_context.c",

            -- Posix / common
            "src/xkb_unicode.c",
            "src/posix_module.c",
            "src/posix_time.c",
            "src/posix_thread.c",
            "src/posix_poll.c",
            "src/egl_context.c",
            "src/osmesa_context.c",
            "src/linux_joystick.c",

            -- Headers for Wayland protocols
            "src/*-client-protocol.h",
            "src/*-client-protocol-code.h",

            -- Optional X11 headers
            "src/x11_platform.h",
            "src/x11_window.h"
        }

        defines {
            "_GLFW_WAYLAND",
            "_GLFW_X11"
        }

        links {
            -- Wayland
            "wayland-client",
            "wayland-cursor",
            "wayland-egl",

            -- X11
            "X11",
            "Xrandr",
            "Xi",
            "Xcursor",
            "Xxf86vm",

            -- Other libs
            "xkbcommon",
            "pthread",
            "dl",
            "m"
        }

    filter "system:windows"
        systemversion "latest"

        files {
            "src/win32_init.c",
            "src/win32_joystick.c",
            "src/win32_module.c",
            "src/win32_monitor.c",
            "src/win32_time.c",
            "src/win32_thread.c",
            "src/win32_window.c",
            "src/wgl_context.c",
            "src/egl_context.c",
            "src/osmesa_context.c"
        }

        defines { 
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter { "system:windows", "configurations:Debug-AS" }    
        runtime "Debug"
        symbols "on"
        sanitize { "Address" }
        flags { "NoRuntimeChecks", "NoIncrementalLink" }

    filter "configurations:Development"
        runtime "Release"
        optimize "speed"

    filter "configurations:Shipping"
        runtime "Release"
        optimize "speed"
        symbols "off"