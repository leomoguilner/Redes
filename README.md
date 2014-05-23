Redes
=====

REDES

Por el momento realiza lo siguiente:
Cliente
Menu e ingreso por pantalla de las diferentes opciones(Consulta de disponiblidad, alta de turno de paciente y baja de turno de paciente) y envio de mensajes con token al servidor.
Faltantes cliente
Faltan validaciones de ingreso y recibir el mensaje de respuesta del server en caso de error o OK

Servidor
Atiende múltiples conexiones utilizando el select
Esta implementada la incializacion del array de turnos con un archivo (faltaria generar mas datos en ese archivo para contar con mas informacion)
Faltantes Servidor 
implementar la consulta,alta y baja.(es solo hacer unas rutinas en c)
Devolver la respuesta al cliente y cerrar sockets.

La consulta recibe fecha ,hora y medico. (debe buscar en el array si para la fecha hora medico indicadas hay un espacio en el array que tenga el valor disponible=1)
El alta recibe fecha,hora, nombre paciente.apellido paciente y medico (baja lo mismo). Solo lo da de alta si hay disponiblidad en el array de turnos para el medico en cuestión (son bajas y altas lógicas)

Si no se entendió algo no tengo problema en explicarlo. Pero lo ideal como dijimos es sacárnoslo de encima rápido. Porque vienen los exámenes en las próximas semanas.
