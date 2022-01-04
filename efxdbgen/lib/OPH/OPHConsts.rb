# frozen_string_literal: true

module EFXDBGen
  module OPH
    module OPHConsts
      MUXNAMES = %w[IMUXA IMUXB IMUXC IMUXD IMUXCE IMUXSR].freeze
      HTRACKIDS = {
        0 => [2, 5, 8, 16, 23, 32, 39, 47],
        1 => [4, 10, 15, 26, 29, 42, 45],
        2 => [0, 7, 13, 24, 31, 40],
        3 => [1, 3, 6, 18, 21, 34, 37],
        4 => [30, 33],
        5 => [36, 37]
      }.freeze
      VTRACKIDS = {
        0 => [4, 7, 11, 22, 33, 38],
        1 => [5, 6, 8, 19, 20, 35, 36],
        2 => [1, 2, 9, 14, 25, 30, 41, 46],
        3 => [3, 12, 17, 27, 28, 43, 44],
        4 => [10, 13],
        5 => [16, 17]
      }.freeze
    end
  end
end
