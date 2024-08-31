        .export _app_data
        .export _app_status
        .export _client_id
        .export _current_step
        .export _client_data_url
        .export _endpoint
        .export _info_display_count
        .export _is_running_sim
        .export _name
        .export _shapes
        .export _shapes_buffer
        .export _shape_count
        .export _url_buffer

        .export _world_width
        .export _world_height
        .export _num_clients
        .export _world_is_frozen
        .export _world_is_wrapped
        .export _body_count
        .export _body_1
        .export _body_2
        .export _body_3
        .export _body_4
        .export _body_5

; only needed for the overall size
.struct ShapeRecord
    shape_id        .res 1
    shape_width     .res 1
    shape_data_len  .res 1
    shape_data      .res 2
.endstruct


.bss

;; GENERAL BUFFERS
_url_buffer:        .res 128
_app_data:          .res 512
_shapes_buffer:     .res 512
_name:              .res 9
_endpoint:          .res 80
_client_data_url:   .res 96

; room for 50 shapes is 50 * 5 = 250 bytes, in BSS so not saved to disk. The data for the shapes strings is stored in _shapes_buffer above.
_shapes:            .res 50 * .sizeof(ShapeRecord)
_shape_count:       .res 1

;; CLIENT INFO
_client_id:         .res 1
; status byte for client
_app_status:        .res 1

;; WORLD FLAGS/DATA
; TODO: make this a struct
_world_width:       .res 2
_world_height:      .res 2
_body_count:        .res 2
_body_1:            .res 1
_body_2:            .res 1
_body_3:            .res 1
_body_4:            .res 1
_body_5:            .res 1
_num_clients:       .res 1
_world_is_frozen:   .res 1
_world_is_wrapped:  .res 1

.data
_is_running_sim:    .byte $01
_current_step:      .byte $ff
_info_display_count: .byte $00
