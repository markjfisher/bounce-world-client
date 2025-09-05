; CoCo (6809) version of data.s
; Exported symbols
	EXPORT _app_data
	EXPORT _app_status
	EXPORT _broadcast_message
	EXPORT _client_data_cmd
	EXPORT _client_data_cmd_len
	EXPORT _client_id
	EXPORT _client_str
	EXPORT _cmd_tmp
	EXPORT _current_step
	EXPORT _clients_buffer
	EXPORT _flash_on_collision
	EXPORT _info_display_count
	EXPORT _is_darkmode
	EXPORT _is_running_sim
	EXPORT _is_showing_broadcast
	EXPORT _is_showing_info
	EXPORT _is_showing_clients
	EXPORT _name
	EXPORT _name_pad
	EXPORT _shapes
	EXPORT _shapes_buffer
	EXPORT _shape_count
	EXPORT _server_url

	EXPORT _world_width
	EXPORT _world_height
	EXPORT _num_clients
	EXPORT _world_is_frozen
	EXPORT _world_is_wrapped
	EXPORT _body_count
	EXPORT _body_1
	EXPORT _body_2
	EXPORT _body_3
	EXPORT _body_4
	EXPORT _body_5

; ---------------------------------------------------------------------------
; ShapeRecord size and layout (ca65 struct was 5 bytes: 1 + 1 + 1 + 2)
; shape_id        .res 1
; shape_width     .res 1
; shape_data_len  .res 1
; shape_data      .res 2
; ---------------------------------------------------------------------------
SHAPE_ID_OFF        EQU 0
SHAPE_WIDTH_OFF     EQU 1
SHAPE_DATA_LEN_OFF  EQU 2
SHAPE_DATA_OFF      EQU 3
SHAPE_RECORD_SIZE  EQU 5

	SECTION bss
; ---------------------------------------------------------------------------
; BSS / uninitialized data (use RMB to reserve bytes)
; ---------------------------------------------------------------------------
; General buffers
	_server_url:        RMB 80
	_app_data:          RMB 256
	_shapes_buffer:     RMB 512
	_name:              RMB 9
	_name_pad:          RMB 1
	_clients_buffer:    RMB 512
	_broadcast_message: RMB 120
	_cmd_tmp:           RMB 64
	_client_data_cmd:   RMB 10
	_client_data_cmd_len: RMB 1

; room for 50 shapes: 50 * SHAPE_RECORD_SIZE
	_shapes:            RMB 50 * SHAPE_RECORD_SIZE
	_shape_count:       RMB 1

; Client info
	_client_id:         RMB 1
	_client_str:        RMB 8

; status byte for client
	_app_status:        RMB 1

; World flags/data
	_world_width:       RMB 2
	_world_height:      RMB 2
	_body_count:        RMB 2
	_body_1:            RMB 1
	_body_2:            RMB 1
	_body_3:            RMB 1
	_body_4:            RMB 1
	_body_5:            RMB 1
	_num_clients:       RMB 1
	_world_is_frozen:   RMB 1
	_world_is_wrapped:  RMB 1

	ENDSECTION

	SECTION initgl
; ---------------------------------------------------------------------------
; Initialized data (originally in .data section)
; use FCB (Form Constant Byte) for initialized bytes
; ---------------------------------------------------------------------------
	_is_running_sim:        FCB $01
	_current_step:          FCB $FF
	_info_display_count:    FCB $00
	_is_darkmode:           FCB $01
	_is_showing_info:       FCB $00
	_is_showing_clients:    FCB $00
	_is_showing_broadcast:  FCB $00
	_flash_on_collision:    FCB $00
	
	ENDSECTION
