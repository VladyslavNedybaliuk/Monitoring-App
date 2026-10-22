load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "monitor",
    srcs = ["src/Monitor.cpp"],
    hdrs = ["inc/Monitor.hpp"],
    includes = ["inc"],
    deps = [":CpuTimes"],
)

cc_library(
    name = "CpuTimes",
    hdrs = ["inc/CpuTimes.hpp"],
    includes = ["inc"],
)

cc_binary(
    name = "main",
    srcs = ["src/main.cpp"],
    deps = [":monitor"],
)
