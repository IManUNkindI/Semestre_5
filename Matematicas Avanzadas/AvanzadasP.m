function afinador_de_guitarra()

    duracion = 3;  % Duración de la grabación en segundos
    frecuencia_muestreo = 44100;  % Frecuencia de muestreo en Hz
    grabacion = [];  % Variable para almacenar la grabación de audio
    cuerda_seleccionada = 1; % Cuerda seleccionada inicialmente
    
    % Crear la interfaz gráfica
    ventana = figure('Name', 'Afinador de guitarra', 'NumberTitle', 'off', 'Position', [100, 100, 400, 200]);
    panel_botones = uipanel('Title', 'Opciones', 'Position', [0.05, 0.05, 0.4, 0.9]);
    boton_grabar = uicontrol('Parent', panel_botones, 'Style', 'pushbutton', 'String', 'Grabar', ...
        'Callback', @iniciar_grabacion, 'Position', [10, 10, 80, 30]);
    menu_cuerdas = uicontrol('Parent', panel_botones, 'Style', 'popupmenu', 'String', {'Mi', 'La', 'Re', 'Sol', 'Si', 'Mi'}, ...
        'Callback', @cambiar_cuerda, 'Position', [120, 10, 80, 30]);
    panel_graficas = uipanel('Title', 'Gráficas', 'Position', [0.55, 0.05, 0.4, 0.9]);
    grafica_senal = axes('Parent', panel_graficas, 'Position', [0.1, 0.55, 0.85, 0.4]);
    grafica_fft = axes('Parent', panel_graficas, 'Position', [0.1, 0.05, 0.85, 0.4]);
    
    % Función para iniciar la grabación de audio
    function iniciar_grabacion(~, ~)
        disp(['Grabando audio de la cuerda ', num2str(cuerda_seleccionada), '...']);
        grabacion = grabar_audio(duracion, frecuencia_muestreo);
        graficar_senal(duracion, grabacion, frecuencia_muestreo);
        calcular_fft();
    end

    % Función para grabar audio
    function grabacion = grabar_audio(duracion, frecuencia_muestreo)
        grabacion = audiorecorder(frecuencia_muestreo, 16, 1);
        recordblocking(grabacion, duracion);
        grabacion = getaudiodata(grabacion);
        disp('Grabación finalizada.');
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
    function calcular_fft()
        if isempty(grabacion)
            disp('Primero graba una señal.');
            return;
        end
        transformada = abs(fft(grabacion));
        frecuencias = linspace(0, frecuencia_muestreo, length(transformada));
        plot(grafica_fft, frecuencias(1:end/2), transformada(1:end/2));
        xlabel(grafica_fft, 'Frecuencia (Hz)');
        ylabel(grafica_fft, 'Magnitud');
        title(grafica_fft, 'Transformada de Fourier');
        ecuacion_fft = ['FFT: Y(f) = ', char(8730), '(X(f))'];
        text(grafica_fft, 0.1, max(transformada)/2, ecuacion_fft);
    end

    % Función para cambiar la cuerda seleccionada
    function cambiar_cuerda(source, ~)
        cuerda_seleccionada = source.Value;
    end

end

