package(default_visibility = ["//visibility:public"])

# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
    name = "sphere-drawer",
    srcs = ["sphere-drawer.cc"],
    hdrs = ["sphere-drawer.h"],
    deps = [
        ":renderer-interface",
        "//galaxy/base:macros",
        "//galaxy/base:vec-type",
        "//galaxy/base:logging",
        "//galaxy/stars/base:sphere",
    ],
    linkopts = [
        "-lglut",
        "-lGL",
    ],
)

# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
    name = "sphere-drawer-manual-test",
    srcs = ["sphere-drawer-manual-test.cc"],
    copts = [],
    deps = [
        ":sphere-drawer",
        "//galaxy:interface-object-registry",
        "//galaxy:context",
        "//galaxy/base:macros",
        "//galaxy/ui:sdl-window",
        "//galaxy/base:logging",
        "//galaxy/base:types",
        "//galaxy/base:vec-type",
        "//galaxy/stars/base:sphere",
    ],
)

cc_library(
    name = "renderer-interface",
    hdrs = ["renderer-interface.h"],
)
