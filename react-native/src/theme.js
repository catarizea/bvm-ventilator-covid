import { DefaultTheme, configureFonts } from 'react-native-paper';

const fontConfig = {
  default: {
    regular: {
      fontFamily: 'Roboto',
      fontWeight: 'normal',
    },
    medium: {
      fontFamily: 'Roboto',
      fontWeight: 'normal',
    },
    light: {
      fontFamily: 'Roboto',
      fontWeight: 'normal',
    },
    thin: {
      fontFamily: 'Roboto',
      fontWeight: 'normal',
    },
  }
};

const theme = {
  ...DefaultTheme,
  dark: true,
  fonts: configureFonts(fontConfig),
  colors: {
    ...DefaultTheme.colors,
    primary: '#2c3385',
  },
};

export default theme;