function afinador_de_guitarra()

    duracion = 3;  % Duración de la grabación en segundos
    frecuencia_muestreo = 44100;  % Frecuencia de muestreo en Hz
    grabacion = [];  % Variable para almacenar la grabación de audio
    cuerda_seleccionada = 1; % Cuerda seleccionada inicialmente
    
    % Frecuencias de afinación estándar para cada cuerda de la guitarra (invirtiendo el orden)
    frecuencias_cuerdas = [329.63, 246.94, 196.00, 146.83, 110.00, 82.41];
    
    % Crear la interfaz gráfica
    ventana = figure('Name', 'Afinador de guitarra', 'NumberTitle', 'off', 'Position', [100, 100, 400, 300]);
    panel_botones = uipanel('Title', 'Opciones', 'Position', [0.05, 0.05, 0.4, 0.9]);
    boton_grabar = uicontrol('Parent', panel_botones, 'Style', 'pushbutton', 'String', 'Grabar', ...
        'Callback', @iniciar_grabacion, 'Position', [10, 10, 80, 30]);
    menu_cuerdas = uicontrol('Parent', panel_botones, 'Style', 'popupmenu', 'String', {'Mi (1ra)', 'Si (2da)', 'Sol (3ra)', 'Re (4ta)', 'La (5ta)', 'Mi (6ta)'}, ...
        'Callback', @cambiar_cuerda, 'Position', [120, 10, 80, 30]);
    texto_resultado = uicontrol('Parent', panel_botones, 'Style', 'text', 'String', '', 'Position', [10, 60, 190, 80], 'HorizontalAlignment', 'left');
    panel_graficas = uipanel('Title', 'Gráficas', 'Position', [0.55, 0.05, 0.4, 0.9]);
    grafica_senal = axes('Parent', panel_graficas, 'Position', [0.1, 0.55, 0.85, 0.4]);
    grafica_fft = axes('Parent', panel_graficas, 'Position', [0.1, 0.05, 0.85, 0.4]);
    
    % Función para iniciar la grabación de audio
    function iniciar_grabacion(~, ~)
        actualizar_texto(['Grabando audio de la cuerda ', num2str(cuerda_seleccionada), '...']);
        grabacion = grabar_audio(duracion, frecuencia_muestreo);
        graficar_senal(duracion, grabacion, frecuencia_muestreo);
        calcular_fft(grabacion);
        frecuencia_fundamental = encontrar_frecuencia_fundamental(grabacion, frecuencia_muestreo);
        actualizar_texto(['Frecuencia fundamental detectada: ', num2str(frecuencia_fundamental), ' Hz']);
        comparar_frecuencia(frecuencia_fundamental);
    end

    % Función para grabar audio
    function audio = grabar_audio(duracion, frecuencia_muestreo)
        recObj = audiorecorder(frecuencia_muestreo, 16, 1);
        recordblocking(recObj, duracion);
        audio = getaudiodata(recObj);
        actualizar_texto('Grabación finalizada.');
    end

    % Función para graficar la señal de audio
    function graficar_senal(tiempo, senal, frecuencia_muestreo)
        tiempo = linspace(0, tiempo, length(senal));  % Eje temporal
        plot(grafica_senal, tiempo, senal);
        xlabel(grafica_senal, 'Tiempo (s)');
        ylabel(grafica_senal, 'Amplitud');
        title(grafica_senal, 'Señal grabada');
    end

    % Función para calcular y graficar la Transformada de Fourier
    function calcular_fft(senal)
        transformada = abs(fft(senal));
        N = length(senal);
        frecuencias = linspace(0, frecuencia_muestreo, N);
        frecuencia_objetivo = frecuencias_cuerdas(cuerda_seleccionada);
        rango_min = max(0, frecuencia_objetivo - 15);
        rango_max = min(frecuencia_muestreo / 2, frecuencia_objetivo + 15);
        indices = (frecuencias >= rango_min) & (frecuencias <= rango_max);
        
        plot(grafica_fft, frecuencias(indices), transformada(indices));
        xlabel(grafica_fft, 'Frecuencia (Hz)');
        ylabel(grafica_fft, 'Magnitud');
        title(grafica_fft, 'Transformada de Fourier');
        xlim(grafica_fft, [rango_min rango_max]);
    end

    % Función para encontrar la frecuencia fundamental usando FFT
    function f0 = encontrar_frecuencia_fundamental(senal, fs)
        N = length(senal);
        transformada = abs(fft(senal));
        frecuencia_objetivo = frecuencias_cuerdas(cuerda_seleccionada);
        rango_min = max(1, round((frecuencia_objetivo - 15) * N / fs));
        rango_max = min(round((frecuencia_objetivo + 15) * N / fs), N/2);
        
        [~, indice_max] = max(transformada(rango_min:rango_max));
        f0 = (rango_min + indice_max - 2) * fs / N; % Ajustar índice al rango real
    end

    % Función para comparar la frecuencia fundamental con la frecuencia de la cuerda seleccionada
    function comparar_frecuencia(f0)
        frecuencia_objetivo = frecuencias_cuerdas(cuerda_seleccionada);
        error = f0 - frecuencia_objetivo;
        mensaje = ['Frecuencia objetivo: ', num2str(frecuencia_objetivo), ' Hz'];
        if abs(error) < 1
            mensaje = [mensaje, '\nLa cuerda está afinada.'];
        elseif error > 0
            mensaje = [mensaje, '\nLa cuerda está alta. Afloje la cuerda.'];
        else
            mensaje = [mensaje, '\nLa cuerda está baja. Apriete la cuerda.'];
        end
        actualizar_texto(mensaje);
    end

    % Función para cambiar la cuerda seleccionada
    function cambiar_cuerda(source, ~)
        cuerda_seleccionada = source.Value;
    end

    % Función para actualizar el texto en el panel de opciones
    function actualizar_texto(mensaje)
        set(texto_resultado, 'String', sprintf(mensaje));
    end

end
