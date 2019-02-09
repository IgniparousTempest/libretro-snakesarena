#!/usr/bin/env bash

# This file is part of The RetroPie Project
#
# The RetroPie Project is the legal property of its developers, whose names are
# too numerous to list here. Please refer to the COPYRIGHT.md file distributed with this source.
#
# See the LICENSE.md file at the top-level directory of this distribution and
# at https://raw.githubusercontent.com/RetroPie/RetroPie-Setup/master/LICENSE.md
#

rp_module_id="lr-supersnakes"
rp_module_desc="Super Snakes - Multiplayer Snake Game"
rp_module_help="https://github.com/IgniparousTempest/libretro-supersnakes/wiki"
rp_module_licence="GPL3 https://raw.githubusercontent.com/IgniparousTempest/libretro-supersnakes/master/LICENSE"
rp_module_section="exp"

function depends_lr-supersnakes() {
    getDepends cmake
}

function sources_lr-supersnakes() {
    gitPullOrClone "$md_build" https://github.com/IgniparousTempest/libretro-supersnakes.git
}

function build_lr-supersnakes() {
    cmake .
    make
    md_ret_require="$md_build/supersnakes_libretro.so"
}

function install_lr-supersnakes() {
    md_ret_files=(
        'supersnakes_libretro.so'
        'resources'
        'LICENSE'
        'README.md'
    )
}

function configure_lr-supersnakes() {
    setConfigRoot "ports"

    addPort "$md_id" "supersnakes" "Super Snakes" "$md_inst/supersnakes_libretro.so"

    ensureSystemretroconfig "ports/supersnakes"
}