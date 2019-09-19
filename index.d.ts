// eslint-disable
declare module "anitomy-js" {
  export interface AnitomyOptions {
    allowed_delimiters?: string;
    ignored_strings?: string[];
    parse_episode_number?: boolean;
    parse_episode_title?: boolean;
    parse_file_extension?: boolean;
    parse_release_group?: boolean;
  }

  export interface AnitomyResult {
    file_name: string;
    anime_season?: string;
    season_prefix?: string;
    anime_title?: string;
    anime_type?: string;
    anime_year?: string;
    audio_term?: string;
    device_compatibility?: string;
    episode_number?: string;
    episode_number_alt?: string;
    episode_prefix?: string;
    episode_title?: string;
    file_checksum?: string;
    file_extension?: string;
    language?: string;
    other?: string;
    release_group?: string;
    release_information?: string;
    release_version?: string;
    source?: string;
    subtitles?: string;
    video_resolution?: string;
    video_term?: string;
    volume_number?: string;
    volume_prefix?: string;
    unknown?: string;
  }

  type AnitomyCallback = (
    error: Error,
    result: AnitomyResult[] | AnitomyResult
  ) => void;

  type AnitomyAsync = (
    input: string | string[],
    options?: AnitomyOptions,
    callback?: AnitomyCallback
  ) => Promise<AnitomyResult[] | AnitomyResult> | undefined;

  type AnitomySync = (
    input: string | string[],
    options?: AnitomyOptions
  ) => AnitomyResult[] | AnitomyResult;

  export const parse: AnitomyAsync;
  export const parseSync: AnitomySync;
}
