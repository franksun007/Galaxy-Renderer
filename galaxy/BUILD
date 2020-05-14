package(default_visibility = ["//visibility:public"])

cc_library(
    name = "context",
    # srcs = ["context.cc"],
    hdrs = ["context.h"],
    deps = [
        ":interface-object-registry",
        "//galaxy/base:logging",
        "//galaxy/base:memory",
        "//galaxy/ui:sdl-window",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "interface-object-registry",
    hdrs = ["interface-object-registry.h"],
)