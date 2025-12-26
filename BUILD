load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

cc_library(
    name = "base64_library",
    srcs = ["base64.h"],
)

cc_binary(
    name = "base64",
    srcs = ["base64.cpp"],
    deps = [
        ":base64_library",
    ],
)
