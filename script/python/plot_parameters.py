""""
This file is used to hold the plot parameter dictionary. This is to be used with plot_experiment.py, and determines 
how the plots are visually represented.
"""""

params = {
    'split-n': 5,  # Number of splits to perform for each member in X space.
    'nibble-plot': {
        'image-bits': 64,  # Number of pixels for heatmap.

        # # Angle plot parameters.
        'nat-xal': r'$\theta_{ij}$',  # X-axis label.

        # Dot Angle (theta_1 and phi) plot parameters.
        'ndat1p-xal': r'$\theta_{ic}$',  # X-axis label.
        'ndat1p-yal': r'$\phi$',  # Y-axis label.
        'ndat1p-xhl': ['0', '5', '10', '15', '20'],  # X-axis histogram tick labels.
        'ndat1p-yhl': ['0', '30', '60', '90', '120', '150', '180'],  # Y-axis histogram tick labels.

        # Dot Angle (theta_2 and phi) plot parameters.
        'ndat2p-xal': r'$\theta_{jc}$',  # X-axis label.
        'ndat2p-yal': r'$\phi$',  # Y-axis label.
        'ndat2p-xhl': ['0', '5', '10', '15', '20'],  # X-axis histogram tick labels.
        'ndat2p-yhl': ['0', '30', '60', '90', '120', '150', '180'],  # Y-axis histogram tick labels.

        # Dot Angle (theta_1 and theta_2) plot parameters.
        'ndat1t2-xal': r'$\theta_{ic}$',  # X-axis label.
        'ndat1t2-yal': r'$\theta_{jc}$',  # Y-axis label.
        'ndat1t2-xhl': ['0', '5', '10', '15', '20'],  # X-axis histogram tick labels.
        'ndat1t2-yhl': ['0', '5', '10', '15', '20'],  # Y-axis histogram tick labels.

        # Planar Triangle plot parameters.
        'nptai-xal': r'$a$',  # X-axis label.
        'nptai-yal': r'$\imath$',  # Y-axis label.
        'nptai-xhl': ['0.01', '0.02', '0.03', '0.04', '0.05'],  # X-axis histogram tick labels.
        'nptai-yhl': ['0.001', '0.002', '0.003', '0.004', '0.005'],  # Y-axis histogram tick labels.

        # Spherical Triangle plot parameters.
        'nstai-xal': r'$a$',  # X-axis label.
        'nstai-yal': r'$\imath$',  # Y-axis label.
        'nstai-xhl': ['0.01', '0.02', '0.03', '0.04', '0.05'],  # X-axis histogram tick labels.
        'nstai-yhl': ['0.001', '0.002', '0.003', '0.004', '0.005'],  # Y-axis histogram tick labels.
    },
    'overlay-plot': {
        'll': ['Angle'],  # Legend list.

        # Sigma4 vs. F1 w/ ShiftDeviation plot parameters.
        's4as-yll': [0, 1.05],  # Y-axis limits.
        's4as-xal': r'$\sigma_4$',  # X axis label.
        's4as-yal': r'$\mathit{TPR}$ (Recall)',  # Y axis label.
        's4as-lxa': 1,  # Set the X axis as logarithmic.
        's4as-nll': 1,  # Do not plot the legend.

        # Sigma4 vs. F1 w/ FalseStars plot parameters.
        's4af-yll': [0, 1.05],  # Y-axis limits.
        's4af-xal': r'$\sigma_4$',  # X axis label.
        's4af-lxa': 1,  # Set the X axis as logarithmic.
        's4af-nll': 1,  # Do not plot the legend.
    },
    'query-sigma-plot': {
        'll': ['Angle', 'Dot', 'Sphere', 'Plane', 'Pyramid'],  # Legend list.

        # Sigma1 vs. SExistence plot parameters.
        's1se-yll': [0, 1.05],  # Y-axis limits.
        's1se-xal': r'$\sigma_1 = \sigma_2 = \ldots = \sigma_n$',  # X axis label.
        's1se-yal': r'$Q$',  # Y axis label.
        's1se-lxa': 1,  # Set the X axis as logarithmic.
        's1se-nll': 1,  # Do not plot the legend.

        # Sigma1 vs. CandidateSetSize plot parameters.
        's1css-yll': [0, 35],  # Y-axis limits.
        's1css-xal': r'$\sigma_1 = \sigma_2 = \ldots = \sigma_n$',  # X axis label.
        's1css-yal': r'$S$',  # Y axis label.
        's1css-lya': 1,  # Set the Y axis as logarithmic.
        's1css-lxa': 1,  # Set the X axis as logarithmic.
        's1css-nll': 1,  # Do not plot the legend.
    },
    'query-plot': {
        'll': ['Angle', 'Dot', 'Sphere', 'Plane', 'Pyramid'],  # Legend list.

        # Shift Deviation vs. SExistence plot parameters.
        'sdse-yll': [0, 1.05],  # Y-axis limits.
        'sdse-xtl': [r'$0$', r'$10^{-4}$', r'$10^{-3}$', r'$10^{-2}$', r'$10^{-1}$'],  # X-axis tick labels.
        'sdse-xal': 'Deviation of Noise (degrees)',  # X-axis label.
        'sdse-yal': r'$Q$',  # Y-axis label.
        'sdse-nll': 1,  # Do not plot the legend.

        # Shift Deviation vs. CandidateSetSize plot parameters.
        'sdcss-yll': [0, 20],  # Y-axis limits.
        'sdcss-xtl': [r'$0$', r'$10^{-4}$', r'$10^{-3}$', r'$10^{-2}$', r'$10^{-1}$'],  # X-axis tick labels.
        'sdcss-xal': 'Deviation of Noise (degrees)',  # X-axis label.
        'sdcss-yal': r'$S$',  # Y-axis label.
        'sdcss-nll': 1,  # Do not plot the legend.

        # Shift Deviation vs. RunningTime plot parameters.
        'sdrt-xtl': [r'$0$', r'$10^{-4}$', r'$10^{-3}$', r'$10^{-2}$', r'$10^{-1}$'],  # X-axis tick labels.
        'sdrt-xal': 'Deviation of Noise (degrees)',  # X-axis label.
        'sdrt-yal': r'Average $t$',  # Y-axis label.
        'sdrt-nll': 1,  # Do not plot the legend.
    },

    'reduction-plot': {
        'll': ['Angle', 'Dot', 'Sphere', 'Plane', 'Pyramid', 'Composite'],  # Legend list.

        # Shift Deviation vs. PercentageCorrect plot parameters.
        'sdpc-yll': [0, 1.05],  # Y-axis limits
        'sdpc-xtl': [r'$0$', r'$10^{-6}$', r'$10^{-5}$', r'$10^{-4}$', r'$10^{-3}$',
                     r'$10^{-2}$'],  # X-axis tick labels.
        'sdpc-xal': 'Deviation of Noise (degrees)',  # X-axis label.
        'sdpc-yal': r"Average $y''$",  # Y-axis label.
        'sdpc-nll': 1,  # Do not plot the legend.

        # Shift Deviation vs. TimeToResult plot parameters.
        'sdttr-xtl': [r'$0$', r'$10^{-6}$', r'$10^{-5}$', r'$10^{-4}$', r'$10^{-3}$',
                     r'$10^{-2}$'],  # X-axis tick labels.
        'sdttr-xal': 'Deviation of Noise (degrees)',  # X-axis label.
        'sdttr-yal': r'Average $t$ (ms)',  # Y-axis label.
        'sdttr-nll': 1,  # Do not plot the legend.

        # False Stars vs. PercentageCorrect plot parameters.
        'fspc-yll': [0, 1.05],  # Y-axis limits.
        'fspc-xtl': ['0', '3', '6', '9', '12'],  # X-axis tick labels.
        'fspc-xal': 'False Star Count',  # X-axis label.
        'fspc-yal': r"Average $y''$",  # Y-axis label.
        'fspc-nll': 1,  # Do not plot the legend.

        # False Stars vs. TimeToResult plot parameters.
        'fsttr-xtl': ['0', '3', '6', '9', '12'],  # X-axis tick labels.
        'fsttr-xal': 'False Star Count',  # X-axis label.
        'fsttr-yal': r'Average $t$ (ms)',  # Y-axis label.
        'fsttr-nll': 1,  # Do not plot the legend.
    },

    'identification-plot': {
        'll': ['Angle', 'Dot', 'Sphere', 'Plane', 'Pyramid', 'Composite'],  # Legend list.
        # 'll': ['Dot', 'Pyramid', 'Composite'],  # Legend list.

        # Shift Deviation vs. PercentageCorrect plot parameters.
        'sdpc-yll': [0, 1.05],  # Y-axis limits
        'sdpc-xtl': [r'$0$', r'$10^{-6}$', r'$10^{-5}$', r'$10^{-4}$', r'$10^{-3}$',
                     r'$10^{-2}$'],  # X-axis tick labels.
        'sdpc-xal': 'Deviation of Noise (degrees)',  # X-axis label.
        'sdpc-yal': r"Average $y'''$",  # Y-axis label.
        'sdpc-nll': 1,  # Do not plot the legend.

        # Shift Deviation vs. TimeToResult plot parameters.
        'sdttr-xtl': [r'$0$', r'$10^{-6}$', r'$10^{-5}$', r'$10^{-4}$', r'$10^{-3}$',
                      r'$10^{-2}$'],  # X-axis tick labels.
        'sdttr-xal': 'Deviation of Noise (degrees)',  # X-axis label.
        'sdttr-yal': r'Average $t$ (ms)',  # Y-axis label.
        'sdttr-nll': 1,  # Do not plot the legend.

        # False Stars vs. PercentageCorrect plot parameters.
        'fspc-yll': [0, 1.05],  # Y-axis limits.
        'fspc-xtl': ['0', '3', '6', '9', '12'],  # X-axis tick labels.
        'fspc-xal': 'False Star Count',  # X-axis label.
        'fspc-yal': r"Average $y'''$",  # Y-axis label.
        'fspc-nll': 1,  # Do not plot the legend.

        # False Stars vs. TimeToResult plot parameters.
        'fsttr-xtl': ['0', '3', '6', '9', '12'],  # X-axis tick labels.
        'fsttr-xal': 'False Star Count',  # X-axis label.
        'fsttr-yal': r'Average $t$ (ms)',  # Y-axis label.
        'fsttr-nll': 1,  # Do not plot the legend.
    }
}
