; std :
#DEFINE print(MESSAGE$) push DS & dataset std-data & alloc $printBuffer~8 & [$printBuffer] = MESSAGE$ & out S8 $printBuffer & pop DS
#SEGMENT std-data
$printBuffer #DL

; One line is either :
;   - [$label] #PREPROCESSOR_KEYWORD ARGUMENTS...
;  or
;   - [$label] [operation arguments...] [; comment]
;


; PROGRAM WITHOUT INCLUDE
;;;;;;
#DEFINE message "Hello world" ; #DEFINE is not required, it is an example

#SEGMENT data
@0 goto $code:$start
@message #DB~message

#SEGMENT code
$start out S8, $message
end

; PROGRAM WITH INCLUDE
;;;;;;
#INCLUDE std

#SEGMENT code
print("Hello world")
end

; "FUNCTIONNAL"
;;;;;;
#SEGMENT data
$0 goto $start
$printBuffer #DL

#SEGMENT code
$start alloc $helloWorld[8]
*$helloWorld = "Hello World"
push $helloWorld
call $print
end

$print 
alloc $tmp[8]
pop $tmp
out *$tmp
return

; std :
$functions
$print                                                  ; void print(S8 print-message-pointer)
$print-message-pointer #DW                              ; {
pop Words[0]                                            ; 
[$print-message-pointer] = Words[0]                     ;   
out S8, $print-message-pointer                          ;   printf("%s", print-message-pointer)
return                                                  ; }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Jeu du plus et du moins
;; TODO RND(A, B)
$start goto $main
$static-data
$argv #DW~256
$messagePrompt #DW~"Devinez le nombre auquel je pense :"
$number #DW
#DEFINE MAX 9999

$main                                                       ; L main(    
in Longs[0]                                                 ;   W argc
in Words[0]                                                 ;   CH8* argv
[$argv] = Words[0]                                          ; ) {
RND(0, MAX)

Words[0] = $message


out Longs[0]                                                
end                                                         ; }
