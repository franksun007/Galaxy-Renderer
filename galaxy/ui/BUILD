package(default_visibility = ["//visibility:public"])

# TODO(Frank): Switch to SDL 2.0
cc_library(
    name = "sdl-window",
    srcs = ["sdl-window.cc"],
    hdrs = ["sdl-window.h"],
    deps = [
        "//galaxy/base:types",
        "//galaxy/base:vec-type",
        "//galaxy/base:logging",
        "//galaxy/opengl:renderer-interface",
        "@com_google_absl//absl/strings",
    ],
    data = ["//textures"],
    linkopts = [
        "-lX11",
        "-lGLU",
        "-lGL",
        "-L/usr/lib/x86_64-linux-gnu",
        "-lSDL",
    ],
)

cc_binary(
    name = "sdl-window-manual-test",
    srcs = ["sdl-window-manual-test.cc"],
    deps = [
        ":sdl-window",
        "//galaxy/base:types",
    ],
)