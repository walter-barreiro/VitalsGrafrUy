classdef app_interfaz_exported < matlab.apps.AppBase

    % Properties that correspond to app components
    properties (Access = public)
        UIFigure               matlab.ui.Figure
        GridLayout             matlab.ui.container.GridLayout
        LeftPanel              matlab.ui.container.Panel
        ECGHRPREditFieldLabel  matlab.ui.control.Label
        ECGHRPREditField       matlab.ui.control.NumericEditField
        IBPmmHgEditFieldLabel  matlab.ui.control.Label
        IBPmmHgEditField       matlab.ui.control.NumericEditField
        SPO2Label_2            matlab.ui.control.Label
        SPO2EditField          matlab.ui.control.NumericEditField
        MonitoringLabel        matlab.ui.control.Label
        VitalsGrafrUyLabel     matlab.ui.control.Label
        StartButton            matlab.ui.control.StateButton
        RightPanel             matlab.ui.container.Panel
        MinSpinnerLabel        matlab.ui.control.Label
        MinSpinnerECG          matlab.ui.control.Spinner
        AlarmsLabel            matlab.ui.control.Label
        MaxSpinnerLabel        matlab.ui.control.Label
        MaxSpinnerECG          matlab.ui.control.Spinner
        TimeLabel              matlab.ui.control.Label
        MinuteSpinnerLabel     matlab.ui.control.Label
        MinuteSpinner          matlab.ui.control.Spinner
        HourSpinnerLabel       matlab.ui.control.Label
        HourSpinner            matlab.ui.control.Spinner
        MinSpinner_7Label      matlab.ui.control.Label
        MinSpinnerIBP          matlab.ui.control.Spinner
        MaxSpinner_6Label      matlab.ui.control.Label
        MaxSpinnerIBP          matlab.ui.control.Spinner
        MinSpinner_8Label      matlab.ui.control.Label
        MinSpinnerSPO2         matlab.ui.control.Spinner
        MaxSpinner_7Label      matlab.ui.control.Label
        MaxSpinnerSP02         matlab.ui.control.Spinner
        ECGHRPRLabel           matlab.ui.control.Label
        IBPmmHgLabel           matlab.ui.control.Label
        SPO2Label              matlab.ui.control.Label
        SetButton              matlab.ui.control.StateButton
    end

    % Properties that correspond to apps with auto-reflow
    properties (Access = private)
        onePanelWidth = 576;
    end

    
    properties (Access = private)
        Propertyapp % Description
    end
    
    
    methods (Access = private)
        
    end
    

    % Callbacks that handle component events
    methods (Access = private)
        
        % Callback function: UIFigure, UIFigure
        function updateAppLayout(app, event)
            position = app.UIFigure.Position;
            
        end

        % Value changed function: MinSpinnerECG
        function MinSpinnerECGValueChanged(app, event)
            valueECGmin = app.MinSpinnerECG.Value;
            assignin('base','valueECGmin',valueECGmin);
            
        end

        % Value changed function: MaxSpinnerECG
        function MaxSpinnerECGValueChanged(app, event)
            valueECGmax = app.MaxSpinnerECG.Value;
            assignin('base','valueECGmax',valueECGmax);
        end

        % Value changed function: MinSpinnerIBP
        function MinSpinnerIBPValueChanged(app, event)
            valueIBPmin = app.MinSpinnerIBP.Value;
            assignin('base','valueIBPmin',valueIBPmin);
        end

        % Value changed function: MaxSpinnerIBP
        function MaxSpinnerIBPValueChanged(app, event)
            valueIBPmax = app.MaxSpinnerIBP.Value;
            assignin('base','valueIBPmax',valueIBPmax);
        end

        % Value changed function: MinSpinnerSPO2
        function MinSpinnerSPO2ValueChanged(app, event)
            valueSPO2min = app.MinSpinnerSPO2.Value;
            assignin('base','valueSPO2min',valueSPO2min);
        end

        % Value changed function: MaxSpinnerSP02
        function MaxSpinnerSP02ValueChanged(app, event)
            valueSPO2max = app.MaxSpinnerSP02.Value;
            assignin('base','valueSPO2max',valueSPO2max);
        end

        % Value changed function: HourSpinner
        function HourSpinnerValueChanged(app, event)
            valueHour = app.HourSpinner.Value;
            assignin('base','valueHour',valueHour);
        end

        % Value changed function: MinuteSpinner
        function MinuteSpinnerValueChanged(app, event)
            valueMinute = app.MinuteSpinner.Value;
            assignin('base','valueMinute',valueMinute);
            
        end

        % Value changed function: ECGHRPREditField
        function ECGHRPREditFieldValueChanged(app, event)
            value = app.ECGHRPREditField.Value;
            
        end

        % Value changed function: IBPmmHgEditField
        function IBPmmHgEditFieldValueChanged(app, event)
            value = app.IBPmmHgEditField.Value;
            
        end

        % Value changed function: SPO2EditField
        function SPO2EditFieldValueChanged(app, event)
            value = app.SPO2EditField.Value
           
        end

        % Value changed function: StartButton
        function StartButtonValueChanged(app, event)
            value = app.StartButton.Value;
            valueECGmin = app.MinSpinnerECG.Value;
            valueECGmin
            assignin('base','valueECGmin',valueECGmin);
            valueECGmax = app.MaxSpinnerECG.Value;
            valueECGmax
            assignin('base','valueECGmax',valueECGmax);
            valueIBPmin = app.MinSpinnerIBP.Value;
            valueIBPmin
            assignin('base','valueIBPmin',valueIBPmin);
            valueIBPmax = app.MaxSpinnerIBP.Value;
            valueIBPmax
            assignin('base','valueIBPmax',valueIBPmax);
            valueSPO2min = app.MinSpinnerSPO2.Value;
            valueSPO2min
            assignin('base','valueSPO2min',valueSPO2min);
            valueSPO2max = app.MaxSpinnerSP02.Value;
            valueSPO2max
            assignin('base','valueSPO2max',valueSPO2max);
            valueHour = app.HourSpinner.Value;
            valueHour
            assignin('base','valueHour',valueHour);
            valueMinute = app.MinuteSpinner.Value;
            valueMinute
            assignin('base','valueMinute',valueMinute); 
            ServerVitals
                        
        end                        
    end

    % Component initialization
    methods (Access = private)

        % Create UIFigure and components
        function createComponents(app)

            % Create UIFigure and hide until all components are created
            app.UIFigure = uifigure('Visible', 'off');
            app.UIFigure.AutoResizeChildren = 'off';
            app.UIFigure.Position = [100 100 666 531];
            app.UIFigure.Name = 'UI Figure';
            app.UIFigure.SizeChangedFcn = createCallbackFcn(app, @updateAppLayout, true);
            app.UIFigure.WindowButtonDownFcn = createCallbackFcn(app, @updateAppLayout, true);

            % Create GridLayout
            app.GridLayout = uigridlayout(app.UIFigure);
            app.GridLayout.ColumnWidth = {237, '1x'};
            app.GridLayout.RowHeight = {'1x'};
            app.GridLayout.ColumnSpacing = 0;
            app.GridLayout.RowSpacing = 0;
            app.GridLayout.Padding = [0 0 0 0];
            app.GridLayout.Scrollable = 'on';

            % Create LeftPanel
            app.LeftPanel = uipanel(app.GridLayout);
            app.LeftPanel.BackgroundColor = [0.9412 0.9412 0.9412];
            app.LeftPanel.Layout.Row = 1;
            app.LeftPanel.Layout.Column = 1;

            % Create ECGHRPREditFieldLabel
            app.ECGHRPREditFieldLabel = uilabel(app.LeftPanel);
            app.ECGHRPREditFieldLabel.HorizontalAlignment = 'right';
            app.ECGHRPREditFieldLabel.FontWeight = 'bold';
            app.ECGHRPREditFieldLabel.Position = [17 392 72 22];
            app.ECGHRPREditFieldLabel.Text = 'ECG HR/PR';

            % Create ECGHRPREditField
            app.ECGHRPREditField = uieditfield(app.LeftPanel, 'numeric');
            app.ECGHRPREditField.ValueChangedFcn = createCallbackFcn(app, @ECGHRPREditFieldValueChanged, true);
            app.ECGHRPREditField.Editable = 'off';
            app.ECGHRPREditField.Tooltip = {''; ''};
            app.ECGHRPREditField.Position = [103 392 84 22];

            % Create IBPmmHgEditFieldLabel
            app.IBPmmHgEditFieldLabel = uilabel(app.LeftPanel);
            app.IBPmmHgEditFieldLabel.HorizontalAlignment = 'right';
            app.IBPmmHgEditFieldLabel.FontWeight = 'bold';
            app.IBPmmHgEditFieldLabel.Position = [5 338 74 22];
            app.IBPmmHgEditFieldLabel.Text = 'IBP (mmHg)';

            % Create IBPmmHgEditField
            app.IBPmmHgEditField = uieditfield(app.LeftPanel, 'numeric');
            app.IBPmmHgEditField.ValueChangedFcn = createCallbackFcn(app, @IBPmmHgEditFieldValueChanged, true);
            app.IBPmmHgEditField.Editable = 'off';
            app.IBPmmHgEditField.Tooltip = {''; ''};
            app.IBPmmHgEditField.Position = [103 338 84 22];

            % Create SPO2Label_2
            app.SPO2Label_2 = uilabel(app.LeftPanel);
            app.SPO2Label_2.HorizontalAlignment = 'right';
            app.SPO2Label_2.FontWeight = 'bold';
            app.SPO2Label_2.Position = [12 278 60 22];
            app.SPO2Label_2.Text = 'SPO2 (%)';

            % Create SPO2EditField
            app.SPO2EditField = uieditfield(app.LeftPanel, 'numeric');
            app.SPO2EditField.ValueChangedFcn = createCallbackFcn(app, @SPO2EditFieldValueChanged, true);
            app.SPO2EditField.Editable = 'off';
            app.SPO2EditField.Tooltip = {''; ''};
            app.SPO2EditField.Position = [103 278 84 22];

            % Create MonitoringLabel
            app.MonitoringLabel = uilabel(app.LeftPanel);
            app.MonitoringLabel.FontSize = 20;
            app.MonitoringLabel.FontWeight = 'bold';
            app.MonitoringLabel.FontColor = [0 0 1];
            app.MonitoringLabel.Position = [47 431 109 24];
            app.MonitoringLabel.Text = 'Monitoring';

            % Create VitalsGrafrUyLabel
            app.VitalsGrafrUyLabel = uilabel(app.LeftPanel);
            app.VitalsGrafrUyLabel.FontSize = 30;
            app.VitalsGrafrUyLabel.FontWeight = 'bold';
            app.VitalsGrafrUyLabel.Position = [13 469 212 41];
            app.VitalsGrafrUyLabel.Text = 'VitalsGrafr Uy';

            % Create StartButton
            app.StartButton = uibutton(app.LeftPanel, 'state');
            app.StartButton.ValueChangedFcn = createCallbackFcn(app, @StartButtonValueChanged, true);
            app.StartButton.Text = 'Start';
            app.StartButton.BackgroundColor = [0.3922 0.8314 0.0745];
            app.StartButton.FontSize = 20;
            app.StartButton.FontWeight = 'bold';
            app.StartButton.Position = [69 177 100 31];

            % Create RightPanel
            app.RightPanel = uipanel(app.GridLayout);
            app.RightPanel.Layout.Row = 1;
            app.RightPanel.Layout.Column = 2;

            % Create MinSpinnerLabel
            app.MinSpinnerLabel = uilabel(app.RightPanel);
            app.MinSpinnerLabel.HorizontalAlignment = 'right';
            app.MinSpinnerLabel.Position = [182 413 25 22];
            app.MinSpinnerLabel.Text = 'Min';

            % Create MinSpinnerECG
            app.MinSpinnerECG = uispinner(app.RightPanel);
            app.MinSpinnerECG.Limits = [10 70];
            app.MinSpinnerECG.ValueChangedFcn = createCallbackFcn(app, @MinSpinnerECGValueChanged, true);
            app.MinSpinnerECG.Position = [222 413 100 22];
            app.MinSpinnerECG.Value = 60;

            % Create AlarmsLabel
            app.AlarmsLabel = uilabel(app.RightPanel);
            app.AlarmsLabel.FontSize = 20;
            app.AlarmsLabel.FontWeight = 'bold';
            app.AlarmsLabel.FontColor = [0 0 1];
            app.AlarmsLabel.Position = [87 442 146 38];
            app.AlarmsLabel.Text = 'Alarms';

            % Create MaxSpinnerLabel
            app.MaxSpinnerLabel = uilabel(app.RightPanel);
            app.MaxSpinnerLabel.HorizontalAlignment = 'right';
            app.MaxSpinnerLabel.Position = [178 373 28 22];
            app.MaxSpinnerLabel.Text = 'Max';

            % Create MaxSpinnerECG
            app.MaxSpinnerECG = uispinner(app.RightPanel);
            app.MaxSpinnerECG.Limits = [90 350];
            app.MaxSpinnerECG.ValueChangedFcn = createCallbackFcn(app, @MaxSpinnerECGValueChanged, true);
            app.MaxSpinnerECG.Position = [221 373 100 22];
            app.MaxSpinnerECG.Value = 120;

            % Create TimeLabel
            app.TimeLabel = uilabel(app.RightPanel);
            app.TimeLabel.FontSize = 20;
            app.TimeLabel.FontWeight = 'bold';
            app.TimeLabel.FontColor = [0 0 1];
            app.TimeLabel.Position = [87 128 130 43];
            app.TimeLabel.Text = 'Time';

            % Create MinuteSpinnerLabel
            app.MinuteSpinnerLabel = uilabel(app.RightPanel);
            app.MinuteSpinnerLabel.HorizontalAlignment = 'right';
            app.MinuteSpinnerLabel.Position = [14 43 42 22];
            app.MinuteSpinnerLabel.Text = 'Minute';

            % Create MinuteSpinner
            app.MinuteSpinner = uispinner(app.RightPanel);
            app.MinuteSpinner.Limits = [0 59];
            app.MinuteSpinner.ValueChangedFcn = createCallbackFcn(app, @MinuteSpinnerValueChanged, true);
            app.MinuteSpinner.Position = [71 43 100 22];
            app.MinuteSpinner.Value = 53;

            % Create HourSpinnerLabel
            app.HourSpinnerLabel = uilabel(app.RightPanel);
            app.HourSpinnerLabel.HorizontalAlignment = 'right';
            app.HourSpinnerLabel.Position = [14 81 32 22];
            app.HourSpinnerLabel.Text = 'Hour';

            % Create HourSpinner
            app.HourSpinner = uispinner(app.RightPanel);
            app.HourSpinner.Limits = [0 24];
            app.HourSpinner.ValueChangedFcn = createCallbackFcn(app, @HourSpinnerValueChanged, true);
            app.HourSpinner.Position = [71 81 100 22];
            app.HourSpinner.Value = 10;

            % Create MinSpinner_7Label
            app.MinSpinner_7Label = uilabel(app.RightPanel);
            app.MinSpinner_7Label.HorizontalAlignment = 'right';
            app.MinSpinner_7Label.Position = [181 317 25 22];
            app.MinSpinner_7Label.Text = 'Min';

            % Create MinSpinnerIBP
            app.MinSpinnerIBP = uispinner(app.RightPanel);
            app.MinSpinnerIBP.Limits = [0 95];
            app.MinSpinnerIBP.ValueChangedFcn = createCallbackFcn(app, @MinSpinnerIBPValueChanged, true);
            app.MinSpinnerIBP.Position = [221 317 100 22];
            app.MinSpinnerIBP.Value = 90;

            % Create MaxSpinner_6Label
            app.MaxSpinner_6Label = uilabel(app.RightPanel);
            app.MaxSpinner_6Label.HorizontalAlignment = 'right';
            app.MaxSpinner_6Label.Position = [178 277 28 22];
            app.MaxSpinner_6Label.Text = 'Max';

            % Create MaxSpinnerIBP
            app.MaxSpinnerIBP = uispinner(app.RightPanel);
            app.MaxSpinnerIBP.Limits = [100 300];
            app.MaxSpinnerIBP.ValueChangedFcn = createCallbackFcn(app, @MaxSpinnerIBPValueChanged, true);
            app.MaxSpinnerIBP.Position = [221 277 100 22];
            app.MaxSpinnerIBP.Value = 110;

            % Create MinSpinner_8Label
            app.MinSpinner_8Label = uilabel(app.RightPanel);
            app.MinSpinner_8Label.HorizontalAlignment = 'right';
            app.MinSpinner_8Label.Position = [182 217 25 22];
            app.MinSpinner_8Label.Text = 'Min';

            % Create MinSpinnerSPO2
            app.MinSpinnerSPO2 = uispinner(app.RightPanel);
            app.MinSpinnerSPO2.Limits = [0 93];
            app.MinSpinnerSPO2.ValueChangedFcn = createCallbackFcn(app, @MinSpinnerSPO2ValueChanged, true);
            app.MinSpinnerSPO2.Position = [222 217 100 22];
            app.MinSpinnerSPO2.Value = 90;

            % Create MaxSpinner_7Label
            app.MaxSpinner_7Label = uilabel(app.RightPanel);
            app.MaxSpinner_7Label.HorizontalAlignment = 'right';
            app.MaxSpinner_7Label.Position = [178 177 28 22];
            app.MaxSpinner_7Label.Text = 'Max';

            % Create MaxSpinnerSP02
            app.MaxSpinnerSP02 = uispinner(app.RightPanel);
            app.MaxSpinnerSP02.Limits = [95 130];
            app.MaxSpinnerSP02.ValueChangedFcn = createCallbackFcn(app, @MaxSpinnerSP02ValueChanged, true);
            app.MaxSpinnerSP02.Position = [221 177 100 22];
            app.MaxSpinnerSP02.Value = 100;

            % Create ECGHRPRLabel
            app.ECGHRPRLabel = uilabel(app.RightPanel);
            app.ECGHRPRLabel.FontWeight = 'bold';
            app.ECGHRPRLabel.Position = [14 392 72 22];
            app.ECGHRPRLabel.Text = 'ECG HR/PR';

            % Create IBPmmHgLabel
            app.IBPmmHgLabel = uilabel(app.RightPanel);
            app.IBPmmHgLabel.FontWeight = 'bold';
            app.IBPmmHgLabel.Position = [14 298 74 22];
            app.IBPmmHgLabel.Text = 'IBP (mmHg)';

            % Create SPO2Label
            app.SPO2Label = uilabel(app.RightPanel);
            app.SPO2Label.FontWeight = 'bold';
            app.SPO2Label.Position = [14 198 60 22];
            app.SPO2Label.Text = 'SPO2 (%)';

            % Create SetButton
           % app.SetButton = uibutton(app.RightPanel, 'state');
           % app.SetButton.ValueChangedFcn = createCallbackFcn(app, @SetButtonValueChanged, true);
           % app.SetButton.Text = 'Set';
           % app.SetButton.BackgroundColor = [0.0745 0.6235 1];
           % app.SetButton.FontSize = 20;
           % app.SetButton.FontWeight = 'bold';
           % app.SetButton.Position = [276 72 100 31];

            % Show the figure after all components are created
            app.UIFigure.Visible = 'on';
        end
    end

    % App creation and deletion
    methods (Access = public)

        % Construct app
        function app = app_interfaz_exported

            % Create UIFigure and components
            createComponents(app)

            % Register the app with App Designer
            registerApp(app, app.UIFigure)

            if nargout == 0
                clear app
            end
        end

        % Code that executes before app deletion
        function delete(app)

            % Delete UIFigure when app is deleted
            delete(app.UIFigure)
        end
    end
end