        .export _app_data
        .export _app_status
        .export _client_id
        .export _current_step
        .export _endpoint
        .export _is_running_sim
        .export _name
        .export _post_data
        .export _shapes
        .export _shapes_buffer
        .export _url_buffer

; typedef struct {
;     uint8_t shape_id;
;     uint8_t shape_width;
; 	uint8_t shape_data_len;
;     uint8_t *shape_data;
; } ShapeRecord;

; only needed for the overall size
.struct ShapeRecord
    shape_id        .res 1
    shape_width     .res 1
    shape_data_len  .res 1
    shape_data      .res 2
.endstruct


.bss

_url_buffer:        .res 128
_app_data:          .res 512
_shapes_buffer:     .res 512
_post_data:         .res 64
_name:              .res 9
_endpoint:          .res 64
_client_id:         .res 1
_app_status:        .res 1

; room for 50 shapes is 50 * 5 = 250 bytes, in BSS so not saved to disk
_shapes:            .res 50 * .sizeof(ShapeRecord)


.data
_is_running_sim:    .byte $01
_current_step:      .byte $ff
