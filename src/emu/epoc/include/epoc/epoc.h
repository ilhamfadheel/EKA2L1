/*
 * Copyright (c) 2018 EKA2L1 Team.
 * 
 * This file is part of EKA2L1 project 
 * (see bentokun.github.com/EKA2L1).
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <common/types.h>

#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <tuple>

namespace eka2l1 {
    namespace common {
        class chunkyseri;
    }

    namespace epoc {
        struct hal;
    }

    class memory_system;
    class manager_system;
    class kernel_system;

    namespace hle {
        class lib_manager;
    }

    class io_system;
    class ntimer;
    class disasm;
    class gdbstub;

    namespace drivers {
        class graphics_driver;
        class audio_driver;
    }

    namespace arm {
        class core;
        using core_instance = std::unique_ptr<core>;
    }

    class debugger_base;
    using hal_instance = std::unique_ptr<epoc::hal>;

    namespace loader {
        struct rom;
    }

    namespace config {
        struct state;
    }

    namespace dispatch {
        struct dispatcher;
    }

    class system_impl;

    /*! A system instance, where all the magic happens. 
     *
     * Represents the Symbian system. You can switch the system version dynamiclly.
    */
    class system {
        // TODO: Make unique
        system_impl *impl;

    public:
        system(const system &) = delete;
        system &operator=(const system &) = delete;

        system(system &&) = delete;
        system &operator=(system &&) = delete;

        system(drivers::graphics_driver *graphics_driver, drivers::audio_driver *audio_driver,
            config::state *conf);

        ~system();

        void set_graphics_driver(drivers::graphics_driver *driver);
        void set_audio_driver(drivers::audio_driver *driver);
        void set_debugger(debugger_base *new_debugger);
        void set_symbian_version_use(const epocver ever);
        void set_cpu_executor_type(const arm_emulator_type type);

        const arm_emulator_type get_cpu_executor_type() const;

        loader::rom *get_rom_info();
        epocver get_symbian_version_use() const;

        void prepare_reschedule();

        void startup();
        bool load(const std::u16string &path, const std::u16string &cmd_arg);

        int loop();
        void shutdown();

        void do_state(common::chunkyseri &seri);

        manager_system *get_manager_system();
        memory_system *get_memory_system();
        kernel_system *get_kernel_system();
        hle::lib_manager *get_lib_manager();
        io_system *get_io_system();
        ntimer *get_ntimer();
        disasm *get_disasm();
        gdbstub *get_gdb_stub();
        drivers::graphics_driver *get_graphics_driver();
        drivers::audio_driver *get_audio_driver();
        arm::core *get_cpu();
        config::state *get_config();
        dispatch::dispatcher *get_dispatcher();

        void set_config(config::state *conf);

        void mount(drive_number drv, const drive_media media, std::string path,
            const std::uint32_t attrib = io_attrib_none);

        void reset();

        bool pause();
        bool unpause();

        bool install_rpkg(const std::string &devices_rom_path, const std::string &path, std::string &firmware_code);
        void load_scripts();

        /*! \brief Set the current device that the system will emulate.
        */
        bool set_device(const std::uint8_t idx);

        /*! \brief Install a SIS/SISX. */
        bool install_package(std::u16string path, drive_number drv);
        bool load_rom(const std::string &path);

        void request_exit();
        bool should_exit() const;

        void add_new_hal(uint32_t hal_category, hal_instance &hal_com);
        epoc::hal *get_hal(uint32_t category);

        const language get_system_language() const;
        void set_system_language(const language new_lang);

        void validate_current_device();
    };
}
