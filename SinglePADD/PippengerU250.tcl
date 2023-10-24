create_project PippengerU250 . -part xcu250-figd2104-2L-e -force
set_property board_part xilinx.com:au250:part0:1.3 [current_project]
add_files -norecurse ./PippengerU250.v
import_files -force -norecurse
update_compile_order -fileset sources_1

ipx::package_project -root_dir ./PippengerU250.srcs -vendor user.org -library user -taxonomy /UserIP

set_property ipi_drc {ignore_freq_hz true} [ipx::current_core]
set_property sdx_kernel true [ipx::current_core]
set_property sdx_kernel_type rtl [ipx::current_core]
set_property vitis_drc {ctrl_protocol user_managed} [ipx::current_core]

ipx::associate_bus_interfaces -busif dataBus -clock clk [ipx::current_core]
ipx::associate_bus_interfaces -busif s_axi_control -clock clk [ipx::current_core]

ipx::add_register start [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
set_property address_offset 0x10 [ipx::get_registers start -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size 32 [ipx::get_registers start -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

ipx::add_register num [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
set_property address_offset 0x18 [ipx::get_registers num -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size 32 [ipx::get_registers num -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

ipx::add_register startReadAddress [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
set_property address_offset 0x20 [ipx::get_registers startReadAddress -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size 64 [ipx::get_registers startReadAddress -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
ipx::add_register_parameter ASSOCIATED_BUSIF [ipx::get_registers startReadAddress -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property value dataBus [ipx::get_register_parameters ASSOCIATED_BUSIF -of_objects [ipx::get_registers startReadAddress -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]]

ipx::add_register startWriteAddress [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
set_property address_offset 0x28 [ipx::get_registers startWriteAddress -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size 64 [ipx::get_registers startWriteAddress -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
ipx::add_register_parameter ASSOCIATED_BUSIF [ipx::get_registers startWriteAddress -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property value dataBus [ipx::get_register_parameters ASSOCIATED_BUSIF -of_objects [ipx::get_registers startWriteAddress -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]]

set_property core_revision 1 [ipx::current_core]
ipx::create_xgui_files [ipx::current_core]
ipx::update_checksums [ipx::current_core]
ipx::check_integrity [ipx::current_core]
ipx::save_core [ipx::current_core]
package_xo  -xo_path ./xo/PippengerU250.xo -kernel_name PippengerU250 -ip_directory ./PippengerU250.srcs -ctrl_protocol user_managed -force
close_project -delete
quit