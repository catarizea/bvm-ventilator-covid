import React from 'react';
import { AppRegistry } from 'react-native';
import App from './src/App';
import { Provider as PaperProvider } from 'react-native-paper';

import { name as appName } from './app.json';
import theme from './src/theme';

const Main = () => (
  <PaperProvider theme={theme}>
    <App />
  </PaperProvider>
);

AppRegistry.registerComponent(appName, () => Main);
