cc_library(
    name = "Constants",
    hdrs = ["Constants.h"],
)

cc_library(
    name = "CumulativeDistributionFunction",
    srcs = ["CumulativeDistributionFunction.cpp"],
    hdrs = ["CumulativeDistributionFunction.h"],
)

cc_library(
    name = "Galaxy",
    srcs = ["Galaxy.cpp"],
    hdrs = ["Galaxy.h"],
    copts = ["-fopenmp"],
    linkopts = ["-fopenmp"],
    deps = [
        ":Constants",
        ":CumulativeDistributionFunction",
        ":Star",
        ":Vector",
    ],
)

cc_library(
    name = "GalaxyProp",
    srcs = ["GalaxyProp.cpp"],
    hdrs = ["GalaxyProp.h"],
)

cc_library(
    name = "NBodyWnd",
    srcs = ["NBodyWnd.cpp"],
    hdrs = ["NBodyWnd.h"],
    deps = [
        ":Constants",
        ":Galaxy",
        ":SDLWnd",
        ":Star",
        "//third_party/specrend",
    ],
)

cc_library(
    name = "OrbitCalculator",
    srcs = ["OrbitCalculator.cpp"],
    hdrs = ["OrbitCalculator.h"],
    deps = [
        ":Constants",
        ":Vector",
    ],
)

# Requires libsdl
# TODO(Frank): Upgrade to 2.0
cc_library(
    name = "SDLWnd",
    srcs = ["SDLWnd.cpp"],
    hdrs = ["SDLWnd.h"],
    linkopts = [
        "-lX11",
        "-lGLU",
        "-lGL",
        "-L/usr/lib/x86_64-linux-gnu",
        "-lSDL",
    ],
    deps = [":Vector"],
)

cc_library(
    name = "Star",
    hdrs = ["Star.h"],
    deps = [
        ":Constants",
        ":OrbitCalculator",
    ],
)

cc_library(
    name = "Vector",
    hdrs = ["Vector.h"],
)

cc_binary(
    name = "Galaxy-Renderer",
    srcs = ["main.cpp"],
    data = ["//textures"],
    deps = [
        ":NBodyWnd",
    ],
)
