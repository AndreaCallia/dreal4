package(default_visibility = ["//visibility:public"])

sh_binary(
    name = "kcov",
    srcs = ["kcov.sh"],
)

sh_binary(
    name = "generate_version_header",
    srcs = [
        "generate_version_header.sh",
    ],
    visibility = ["//visibility:public"],
)

sh_binary(
    name = "generate_pkg_file_osx",
    srcs = [
        "generate_pkg_file_osx.sh",
    ],
    visibility = ["//visibility:public"],
)

sh_binary(
    name = "generate_pkg_file_ubuntu",
    srcs = [
        "generate_pkg_file_ubuntu.sh",
    ],
    visibility = ["//visibility:public"],
)

# === CROSSTOOL rules ===

# The default toolchain selector for the project.
#
# On OS X, we do not support any custom values for the --compiler
# flag. It must be unspecified.  The CROSSTOOL file maps this to the
# system default Clang via osx_cc_wrapper.sh.  Confusingly, Clang is
# installed on Mac as /usr/bin/gcc as well as /usr/bin/clang, and the
# wrapper script invokes the former.
#
# On Linux, we support --compiler strings "clang-3.9", "clang-4.0",
# "clang-5.0", "clang-6.0", "gcc-5", "gcc-6", "gcc-7", and "gcc-8"
# each of which does exactly what you would expect. "gcc-5" is the
# default for convenience, because it is shipped with Xenial.
cc_toolchain_suite(
    name = "default-toolchain",
    toolchains = {
        "darwin_x86_64|compiler": "cc_toolchain_apple",
        "k8|clang-3.9": "cc_toolchain_linux",
        "k8|clang-4.0": "cc_toolchain_linux",
        "k8|clang-5.0": "cc_toolchain_linux",
        "k8|clang-6.0": "cc_toolchain_linux",
        "k8|gcc-5": "cc_toolchain_linux",
        "k8|gcc-6": "cc_toolchain_linux",
        "k8|gcc-7": "cc_toolchain_linux",
        "k8|gcc-8": "cc_toolchain_linux",
    },
)

filegroup(
    name = "empty",
    srcs = [],
    visibility = ["//visibility:private"],
)

# A compiler wrapper script that adjusts linker paths on OS X.
filegroup(
    name = "osx_cc_wrapper",
    srcs = [
        "osx_cc_wrapper.sh",
        "//third_party:com_github_bazelbuild_bazel/tools/cpp/osx_cc_wrapper.sh",
    ],
    visibility = ["//visibility:private"],
)

filegroup(
    name = "osx_ar_wrapper",
    srcs = ["osx_ar_wrapper.sh"],
    visibility = ["//visibility:private"],
)

filegroup(
    name = "apple_toolchain_deps",
    srcs = [
        ":osx_ar_wrapper",
        ":osx_cc_wrapper",
    ],
    visibility = ["//visibility:private"],
)

filegroup(
    name = "linux_toolchain_deps",
    visibility = ["//visibility:private"],
)

cc_toolchain(
    name = "cc_toolchain_linux",
    all_files = ":linux_toolchain_deps",
    compiler_files = ":linux_toolchain_deps",
    cpu = "k8",
    dwp_files = ":linux_toolchain_deps",
    dynamic_runtime_libs = [":empty"],
    linker_files = ":linux_toolchain_deps",
    objcopy_files = ":linux_toolchain_deps",
    static_runtime_libs = [":empty"],
    strip_files = ":linux_toolchain_deps",
    supports_param_files = 0,
)

cc_toolchain(
    name = "cc_toolchain_apple",
    all_files = ":apple_toolchain_deps",
    compiler_files = ":apple_toolchain_deps",
    cpu = "darwin_x86_64",
    dwp_files = ":apple_toolchain_deps",
    dynamic_runtime_libs = [":empty"],
    linker_files = ":apple_toolchain_deps",
    objcopy_files = ":apple_toolchain_deps",
    static_runtime_libs = [":empty"],
    strip_files = ":apple_toolchain_deps",
    supports_param_files = 0,
)

# === config_setting rules ===

config_setting(
    name = "debug",
    values = {"compilation_mode": "dbg"},
)

config_setting(
    name = "with_capd",
    values = {"define": "WITH_CAPD=ON"},
)

config_setting(
    name = "linux",
    values = {"cpu": "k8"},
)

config_setting(
    name = "apple",
    values = {"cpu": "darwin_x86_64"},
)

config_setting(
    name = "gcc8-linux",
    values = {
        "compiler": "gcc-8",
        "cpu": "k8",
    },
)

config_setting(
    name = "gcc7-linux",
    values = {
        "compiler": "gcc-7",
        "cpu": "k8",
    },
)

config_setting(
    name = "gcc6-linux",
    values = {
        "compiler": "gcc-6",
        "cpu": "k8",
    },
)

config_setting(
    name = "gcc5-linux",
    values = {
        "compiler": "gcc-5",
        "cpu": "k8",
    },
)

config_setting(
    name = "clang6.0-linux",
    values = {
        "compiler": "clang-6.0",
        "cpu": "k8",
    },
)

config_setting(
    name = "clang5.0-linux",
    values = {
        "compiler": "clang-5.0",
        "cpu": "k8",
    },
)

config_setting(
    name = "clang4.0-linux",
    values = {
        "compiler": "clang-4.0",
        "cpu": "k8",
    },
)

config_setting(
    name = "clang3.9-linux",
    values = {
        "compiler": "clang-3.9",
        "cpu": "k8",
    },
)

config_setting(
    name = "apple_debug",
    values = {
        "compilation_mode": "dbg",
        "cpu": "darwin_x86_64",
    },
)
