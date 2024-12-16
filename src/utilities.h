/*
  ==============================================================================

    utilities.h
    Created: 19 Nov 2023 7:35:03pm
    Author:  hingo

  ==============================================================================
*/

#pragma once

namespace util
{
  // web-formatted color utiltity: provide a hex code and a numeric alpha value together
  auto hexA = [](uint32_t h, float a)
  {
    return juce::Colour(0xff000000 | h).withAlpha(a);
  };

  // these are some colors from the Tailwind color system:
  // https://tailwindcss.com/docs/customizing-colors
  enum tw
  {
    TW_ZINC_950 = 0x09090b,
    TW_ZINC_900 = 0x18181b,
    TW_ZINC_800 = 0x27272a,
    TW_ZINC_700 = 0x3f3f46,
    TW_ZINC_600 = 0x52525b,
    TW_ZINC_500 = 0x71717a,
    TW_ZINC_400 = 0xa1a1aa,
    TW_ZINC_300 = 0xd4d4d8,
    TW_ZINC_200 = 0xe4e4e7,
    TW_ZINC_100 = 0xf4f4f5,
    TW_ZINC_050 = 0xfafafa,

    TW_RED_600 = 0xdc2626,
    TW_AMBER_600 = 0xd97706,
    TW_YELLOW_600 = 0xca8a04,
    TW_GREEN_600 = 0x16a34a,
    TW_EMERALD_600 = 0x059669,
    TW_EMERALD_700 = 0x047857,
    TW_SKY_600 = 0x0284c7,
    TW_SKY_500 = 0x0ea5e9,
    TW_VIOLET_600 = 0x7c3aed
  };
}
